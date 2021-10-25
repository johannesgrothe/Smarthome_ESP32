#pragma once

#include <utility>

#include "../gadgets/sensor_motion.h"

class GadgetSensorMotionDummy : public SH_Sensor_Motion {
public:
  explicit GadgetSensorMotionDummy(std::string name);

  void refresh() final;
};

GadgetSensorMotionDummy::GadgetSensorMotionDummy(std::string name):
  SH_Sensor_Motion(std::move(name)) {}

void GadgetSensorMotionDummy::refresh() {}