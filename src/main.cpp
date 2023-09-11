#include "server.h"
#include "micro_logger.hpp"
#include "micro_logger_writer.hpp"

#include <memory>

std::shared_ptr<micro_logger::BaseWriter> writer;

void set_stdo_writer() {
  writer = std::make_shared<micro_logger::StandardOutWriter>();
  set_writer(*writer);
  MSG_INFO("Logger has been initialize");
};

int main(int argc, char **argv) {
  set_stdo_writer();
  auto& server = Server::getInstance();
  server.wait();
  return 0;
}
