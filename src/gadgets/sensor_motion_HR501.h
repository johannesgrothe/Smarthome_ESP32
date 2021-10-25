#pragma once

#include "sensor_motion.h"

class Sensor_Motion_HR501 : public Sensor_Motion {
private:
  int input_pin_;

public:
  /**
   * Constructor for class Sensor_Motion_HR501
   * @param gadget
   */
  explicit Sensor_Motion_HR501(std::string name, uint8_t pin);

  void refresh() override;

};

static std::shared_ptr<Sensor_Motion_HR501> create_SensorMotionHR501(std::string name, port_set pins, const JsonObject& gadget_data) {
if (pins[0] != 0) {
    return std::make_shared<Sensor_Motion_HR501>(name, pins[0]);
  } else {
    logger_e("MotionSensorHR501", "No pin set");
    return nullptr;
  }
}
