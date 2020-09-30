#include "sh_sensor_temperature_DHT.h"

SH_Sensor_Temperature_DHT::SH_Sensor_Temperature_DHT(const JsonObject &gadget) :
  SH_Sensor_Temperature(gadget),
  sensor_(nullptr){
  if (gadget["pin"] != nullptr) {
    pin_ = gadget["pin"].as<uint8_t>();
  } else {
    pin_ = 0;
  }
  if (pin_ != 0) {
    sensor_ = new DHT(pin_, DHT22);
    sensor_->begin();
  }
}

void SH_Sensor_Temperature_DHT::refresh() {
  if (sensor_ != nullptr) {
    setHumidity((int) sensor_->readHumidity());
    setTemperature((int) sensor_->readTemperature());
  }
}