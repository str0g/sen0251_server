#ifndef __SERVER_H__
#define __SERVER_H__

#include <vector>
#include "listner.h"

class Server {
  public:
    static Server& getInstance();
    void wait();
    ~Server();
  private:
    Server();
    Server(Server&&) = delete;
    Server(const Server&) = delete;
    Server& operator=(Server&&) = delete;
    Server& operator=(const Server&) = delete;

    std::vector<Listner*> listeners;
    std::string path = "/tmp/sen0251";
};

#endif // !__SERVER_H__
