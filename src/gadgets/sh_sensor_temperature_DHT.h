#pragma once

#include "sh_sensor_temperature.h"
#include <DHT.h>

class SH_Sensor_Temperature_DHT : public SH_Sensor_Temperature {
private:
  uint8_t pin_;
  DHT *sensor_;

public:
  /**
   * constructor for class SH_Sensor_Temperature_DHT
   * @param name Name of the gadget
   * @param pin Pin for the sensor
   */
  explicit SH_Sensor_Temperature_DHT(std::string name, uint8_t pin);

  void refresh() override;
};

static std::shared_ptr<SH_Sensor_Temperature_DHT> createSHSensorTemperatureDHT(std::string name, port_set pins, const JsonObject& gadget_data) {
  if (pins[0] != 0) {
    logger_i("SensorTemperatureDHT", "Pin: %d", pins[0]);
    return std::make_shared<SH_Sensor_Temperature_DHT>(name, pins[0]);
  } else {
    logger_e("SensorTemperatureDHT", "No pin set");
    return nullptr;
  }
}
