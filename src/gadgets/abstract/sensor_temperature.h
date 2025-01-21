#pragma once

#include "gadget.h"

class Sensor_Temperature : public Gadget {
  int temp_;
  int humidity_;

protected:
  void setTemperature(int temp);

  void setHumidity(int hum);

public:
  /**
   * constructor for class Sensor_Temperature
   * @param name Name for the gadget
   */
  explicit Sensor_Temperature(std::string name);
};