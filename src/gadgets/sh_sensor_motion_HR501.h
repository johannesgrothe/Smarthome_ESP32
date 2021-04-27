#pragma once

#include "sh_sensor_motion.h"

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

static std::shared_ptr<SH_Sensor_Motion_HR501> createSHSensorMotionHR501(std::string name, pin_set pins, const JsonObject& gadget_data) {
  uint8_t pin = 0;

  if (pins[0] != 0) {
    pin = pins[0];
  } else {
    logger.sender("MotionSensorHR501").level(LOG_TYPE::ERR) << "No pin set";
    return nullptr;
  }

  logger.sender("MotionSensorHR501") << "Pin: " << pin << "\n";

  return std::make_shared<SH_Sensor_Motion_HR501>(name, pin);
}