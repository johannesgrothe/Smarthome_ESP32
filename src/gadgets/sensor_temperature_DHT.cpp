#include "sensor_temperature_DHT.h"

#include <utility>

Sensor_Temperature_DHT::Sensor_Temperature_DHT(std::string name, uint8_t pin) :
    Sensor_Temperature(std::move(name)),
    pin_(pin),
    sensor_(nullptr) {
  if (pin_ == 0) {
    updateInitStatus(false);
  } else {
    sensor_ = new DHT(pin_, DHT22);
    sensor_->begin();
  }
}

void Sensor_Temperature_DHT::refresh() {
  if (sensor_ != nullptr) {
    setHumidity((int) sensor_->readHumidity());
    setTemperature((int) sensor_->readTemperature());
  }
}
