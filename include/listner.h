#ifndef __LISTENER_H__
#define __LISTENER_H__

#include <string>
#include <thread>
#include <atomic>
#
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <string.h>

class Listner {
  public:
    explicit Listner(const std::string& path, const std::string& file);
    virtual ~Listner();
    void stop();
    bool is_stopped() const;

    Listner(const Listner&) = delete;
    Listner(Listner&&) = delete;
    Listner& operator=(const Listner&) = delete;
    Listner& operator=(Listner&&) = delete;
  protected:
    virtual int get_reading() = 0;
    void runner();
    std::atomic<bool> is_running;
    std::string path;
    char buf[16] = {};
  private:
    void clean_up();
    std::thread th;
    struct sockaddr_un name = {};
    int sock;
};

#endif
