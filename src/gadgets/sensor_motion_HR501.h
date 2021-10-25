#pragma once

#include "sensor_motion.h"

class SH_Sensor_Motion_HR501 : public SH_Sensor_Motion {
private:
  int input_pin_;

public:
  /**
   * Constructor for class SH_Sensor_Motion_HR501
   * @param gadget
   */
  explicit SH_Sensor_Motion_HR501(std::string name, uint8_t pin);

  void refresh() override;

};

static std::shared_ptr<SH_Sensor_Motion_HR501> createSHSensorMotionHR501(std::string name, port_set pins, const JsonObject& gadget_data) {
if (pins[0] != 0) {
    return std::make_shared<SH_Sensor_Motion_HR501>(name, pins[0]);
  } else {
    logger_e("MotionSensorHR501", "No pin set");
    return nullptr;
  }
}
