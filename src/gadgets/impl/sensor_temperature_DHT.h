#pragma once

#include "../abstract/sensor_temperature.h"
#include <DHT.h>

class Sensor_Temperature_DHT final : public Sensor_Temperature {

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
