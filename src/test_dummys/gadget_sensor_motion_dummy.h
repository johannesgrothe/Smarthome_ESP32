#pragma once

#include "../gadgets/sh_sensor_motion.h"

class GadgetSensorMotionDummy : public SH_Sensor_Motion {
public:
  GadgetSensorMotionDummy(std::string name);

  void refresh();
};

GadgetSensorMotionDummy::GadgetSensorMotionDummy(std::string name):
  SH_Sensor_Motion(name) {}

void GadgetSensorMotionDummy::refresh() {}