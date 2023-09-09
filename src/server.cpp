#include <filesystem>
#include <exception>
#include <chrono>

#include "micro_logger.hpp"

#include "server.h"
#include "listners.h"

using namespace std::chrono_literals;

Server& Server::getInstance() {
  static Server server;
  return server;
}

Server::Server() {
  try {
    if (std::filesystem::exists(lock_file)) {
      throw std::runtime_error("another instance is running");
    }
    std::filesystem::create_directory(path);
    std::filesystem::permissions(path,
                                 std::filesystem::perms::owner_all |
                                     std::filesystem::perms::group_read | std::filesystem::perms::group_exec |
                                     std::filesystem::perms::others_read | std::filesystem::perms::others_exec);
    listener_resources_init();
  } catch (const std::filesystem::filesystem_error& e) {
    MSG_ERROR("%s", e.what());
    return;
  } catch (const std::runtime_error& e) {
    MSG_ERROR("%s", e.what());
    return;
  }

  listeners.emplace_back(new ListnerTemperature(path));
  listeners.emplace_back(new ListnerPressure(path));
  listeners.emplace_back(new ListnerAltitiude(path));
  MSG_INFO("service is on-line");
  std::ofstream fs(lock_file);
  std::filesystem::permissions(lock_file, std::filesystem::perms::owner_write);
}

void Server::wait() {
  auto cnt = listeners.size();
  auto tmp = cnt;
  do {
    tmp = cnt;
    for (auto obj:listeners) {
      if (obj->is_stopped()) {
        --tmp;
      }
    }
    std::this_thread::sleep_for(1500ms);
  } while (tmp);
}

Server::~Server() {
  MSG_INFO("shutdown");
  for(auto obj:listeners) {
    delete obj;
  }

  auto removed = std::filesystem::remove_all(path);
  if(removed > 1) {
    MSG_INFO("removed %ld", removed);
  }

  std::filesystem::remove(lock_file);
}

