#include "sh_sensor_motion_HR501.h"

SH_Sensor_Motion_HR501::SH_Sensor_Motion_HR501(const JsonObject &gadget) :
  SH_Sensor_Motion(gadget),
  input_pin_(0) {
  if (gadget["pin"] != nullptr) {
    input_pin_ = gadget["pin"].as<uint8_t>();
  }
}


void SH_Sensor_Motion_HR501::refresh() {
  if (input_pin_ != 0){
    int val = digitalRead(input_pin_);
    setStatus(val);
  }
}