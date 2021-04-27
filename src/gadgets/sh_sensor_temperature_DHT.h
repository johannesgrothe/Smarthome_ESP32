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

static std::shared_ptr<SH_Sensor_Temperature_DHT> createSHSensorTemperatureDHT(std::string name, pin_set pins, const JsonObject& gadget_data) {
  uint8_t pin = 0;

  if (pins[0] != 0) {
    pin = pins[0];
  } else {
    logger.log("SensorTemperatureDHT", LOG_TYPE::ERR) << "No pin set\n";
    return nullptr;
  }

  logger.sender("DoorbellBasic") << "Pin: " << pin << "\n";

  return std::make_shared<SH_Sensor_Temperature_DHT>(name, pin);
}