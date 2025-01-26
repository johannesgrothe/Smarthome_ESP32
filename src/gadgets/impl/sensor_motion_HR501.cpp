#include "sensor_motion_HR501.h"

#include <hardware_controller.h>
#include <utility>

gadget_definitions::GadgetIdentifier Sensor_Motion_HR501::encodeType() {
  return gadget_definitions::GadgetIdentifier::sensor_motion_hr501;
}

Sensor_Motion_HR501::Sensor_Motion_HR501(std::string name, uint8_t pin) :
    Sensor_Motion(std::move(name)),
    input_pin_(pin) {
  HardwareController::setPinMode(pin, INPUT);
}

void Sensor_Motion_HR501::refresh() {
  const int val = HardwareController::digitalReadPin(input_pin_);
  setStatus(val);
}