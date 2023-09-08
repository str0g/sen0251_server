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
    std::filesystem::create_directory(path);
    listener_resources_init();
  } catch (const std::filesystem::filesystem_error& e) {
    MSG_INFO("%s", e.what());
    return;
  }

  listeners.emplace_back(new ListnerTemperature(path));
  listeners.emplace_back(new ListnerPressure(path));
  listeners.emplace_back(new ListnerAltitiude(path));
  MSG_INFO("service is on-line");
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
}

