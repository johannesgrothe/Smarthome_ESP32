#pragma once

#include <utility>

#include "../gadgets/sensor_motion.h"

class GadgetSensorMotionDummy : public Sensor_Motion {
public:
  explicit GadgetSensorMotionDummy(std::string name);

  void refresh() final;
};

GadgetSensorMotionDummy::GadgetSensorMotionDummy(std::string name):
    Sensor_Motion(std::move(name)) {}

void GadgetSensorMotionDummy::refresh() {}