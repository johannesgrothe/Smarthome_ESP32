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
   * @param gadget
   */
  explicit SH_Sensor_Temperature_DHT(const JsonObject &gadget);

  void refresh() override;
};
