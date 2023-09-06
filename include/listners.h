#ifndef __LISTNERS_H__
#define __LISTNERS_H__

#include "listner.h"

class ListnerTemperature: public Listner {
  public:
    explicit ListnerTemperature(const std::string& path) : Listner(path, "temperature.sock"){};
  protected:
    int get_reading() final; 
};

class ListnerPressure: public Listner {
  public:
    explicit ListnerPressure(const std::string& path) : Listner(path, "pressure.sock"){};
  protected:
    int get_reading() final;
};

class ListnerAltitiude: public Listner {
  public:
    explicit ListnerAltitiude(const std::string& path) : Listner(path, "altitude.sock"){};
  protected:
    int get_reading() final;
};

#endif
