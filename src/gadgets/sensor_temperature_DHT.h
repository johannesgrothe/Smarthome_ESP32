#pragma once

#include "sensor_temperature.h"
#include <DHT.h>

class Sensor_Temperature_DHT : public Sensor_Temperature {
private:
  uint8_t pin_;
  DHT *sensor_;

public:
  /**
   * constructor for class Sensor_Temperature_DHT
   * @param name Name of the gadget
   * @param pin Pin for the sensor
   */
  explicit Sensor_Temperature_DHT(std::string name, uint8_t pin);

  void refresh() override;
};

static std::shared_ptr<Sensor_Temperature_DHT> create_SensorTemperatureDHT(std::string name, port_set pins, const JsonObject& gadget_data) {
  if (pins[0] != 0) {
    logger_i("SensorTemperatureDHT", "Pin: %d", pins[0]);
    return std::make_shared<Sensor_Temperature_DHT>(name, pins[0]);
  } else {
    logger_e("SensorTemperatureDHT", "No pin set");
    return nullptr;
  }
}
