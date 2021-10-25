#pragma once

#include "../gadgets/sensor_temperature.h"

class GadgetSensorTemperatureDummy : public SH_Sensor_Temperature {
public:
  GadgetSensorTemperatureDummy(std::string name);

  void refresh();
};

GadgetSensorTemperatureDummy::GadgetSensorTemperatureDummy(std::string name):
  SH_Sensor_Temperature(name) {}

void GadgetSensorTemperatureDummy::refresh() {}