#include "listner.h"

#include <filesystem>
#include "micro_logger.hpp"

using namespace std::chrono_literals;

Listner::Listner(const std::string& path, const std::string& file): path(path+"/"+file), is_running(false), sock(-1) {
  clean_up();
  std::thread tmp([this](){runner();});
  tmp.swap(th);
  std::this_thread::sleep_for(100ms);
}

Listner::~Listner() {
  if(th.joinable())
    th.join();
  if(sock != -1)
    close(sock);
  clean_up();
}

void Listner::stop() {
  is_running = false;
  auto s = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (s == -1) {
    MSG_ERROR("%s", strerror(errno));
    return;
  }
  struct sockaddr_un n = {};
  n.sun_family = AF_UNIX;
  strncpy(n.sun_path, path.c_str(), path.size());
  int rc = connect(s, (struct sockaddr*)&n, sizeof(n));
  if(rc == -1) {
    MSG_ERROR("%s", strerror(errno));
    return;
  }
  close(s);
}

bool Listner::is_stopped() const {
  return !is_running;
}

void Listner::clean_up() {
  std::filesystem::remove(this->path);
}

void Listner::runner() {
  is_running = true;
  sock = socket(AF_UNIX, SOCK_SEQPACKET, 0);
  if (sock == -1) {
    MSG_ERROR("%s", strerror(errno));
    return;
  }
  name.sun_family = AF_UNIX;
  if(sizeof(name.sun_path) < path.size()) {
    MSG_ERROR("name too long refuse to do anything");
    return;
  }
  strncpy(name.sun_path, path.c_str(), path.size());
  int rc = bind(sock, (const struct sockaddr *)&name, sizeof(name));
  if(rc == -1) {
    MSG_ERROR("%s", strerror(errno));
    return;
  }
  try {
    std::filesystem::permissions(path,
                                 std::filesystem::perms::owner_all |
                                     std::filesystem::perms::group_read | std::filesystem::perms::group_write |
                                     std::filesystem::perms::others_read | std::filesystem::perms::others_write);
  } catch (const std::filesystem::filesystem_error& e) {
    MSG_ERROR("%s", e.what());
    return;
  }

  rc = listen(sock, 10);
  if (rc == -1) {
    MSG_ERROR("%s", strerror(errno));
    return;
  }

  while(is_running) {
    MSG_DEBUG("waiting for client");
    int connection = accept(sock, NULL, NULL);
    write(connection, buf, this->get_reading());
    close(connection);
  }
  MSG_DEBUG("runner has ended");
}

