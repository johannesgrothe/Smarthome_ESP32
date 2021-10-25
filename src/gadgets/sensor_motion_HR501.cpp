#include "sensor_motion_HR501.h"

#include <utility>

SH_Sensor_Motion_HR501::SH_Sensor_Motion_HR501(std::string name, uint8_t pin) :
  SH_Sensor_Motion(std::move(name)),
  input_pin_(pin) {
  pinMode(input_pin_, INPUT);
}

void SH_Sensor_Motion_HR501::refresh() {
  int val = digitalRead(input_pin_);
  setStatus(val);
}