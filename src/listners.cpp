#include "listners.h"
#
#include <stdexcept>
#
#include "sen0251.h"

Sen0251::Readings lazy_reading() {
  static Sen0251 sensor([]{
    const char *i2c_dev_id = std::getenv(I2C_DEV_ID);
    if(!i2c_dev_id) {
      throw std::invalid_argument("expected " + std::string(I2C_DEV_ID) + " environment variable");
    }
    return std::stoi(i2c_dev_id);
  }());
  return sensor.get_readings();
}

void listener_resources_init() {
  lazy_reading();
}

int ListnerTemperature::get_reading() {
  static std::string str {'C'};
  auto tmp = std::to_string(lazy_reading().temperature) + str;
  strncpy(buf,tmp.c_str(),tmp.size());
  return tmp.size() + str.size();
}

int ListnerPressure::get_reading() {
  static std::string str {"hpa"};
  auto tmp = std::to_string(lazy_reading().pressure) + str;
  strncpy(buf,tmp.c_str(),tmp.size());
  return tmp.size() + str.size();
}

int ListnerAltitiude::get_reading() {
  static std::string str {"m"};
  auto tmp = std::to_string(lazy_reading().altitude) + str;
  strncpy(buf,tmp.c_str(),tmp.size());
  return tmp.size() + str.size();
}
