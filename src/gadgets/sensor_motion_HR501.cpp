#include "sensor_motion_HR501.h"

#include <utility>

Sensor_Motion_HR501::Sensor_Motion_HR501(std::string name, uint8_t pin) :
    Sensor_Motion(std::move(name)),
    input_pin_(pin) {
  pinMode(input_pin_, INPUT);
}

void Sensor_Motion_HR501::refresh() {
  int val = digitalRead(input_pin_);
  setStatus(val);
}