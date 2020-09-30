#pragma once

#include "sh_sensor_temperature.h"
#include <DHT.h>

class SH_Sensor_Temperature_DHT : public SH_Sensor_Temperature {
private:
  uint8_t pin_;
  DHT *sensor_;

public:
  explicit SH_Sensor_Temperature_DHT(const JsonObject&);

  void refresh() override;
};
