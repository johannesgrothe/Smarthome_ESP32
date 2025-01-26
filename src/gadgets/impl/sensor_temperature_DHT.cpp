#include "sensor_temperature_DHT.h"

#include <utility>

gadget_definitions::GadgetIdentifier Sensor_Temperature_DHT::encodeType() {
  return gadget_definitions::GadgetIdentifier::sensor_temperature_dht;
}

Sensor_Temperature_DHT::Sensor_Temperature_DHT(std::string name, const uint8_t pin) :
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
    setHumidity(static_cast<int>(sensor_->readHumidity()));
    setTemperature(static_cast<int>(sensor_->readTemperature()));
  }
}
