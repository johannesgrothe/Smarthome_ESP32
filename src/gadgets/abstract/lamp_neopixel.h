#pragma once

#include "lamp_rgb.h"
#include <Adafruit_NeoPixel.h>

class Lamp_NeoPixel_RGB : public Lamp_RGB {
  uint8_t pin_;
  uint16_t len_;
  Adafruit_NeoPixel led_stripe_;

protected:
  bool setLedColor(uint8_t, uint8_t, uint8_t);

public:
  Lamp_NeoPixel_RGB(std::string name, uint8_t pin, uint16_t len);

};
