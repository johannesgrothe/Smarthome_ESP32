#pragma once

#include "sh_lamp.h"
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

class SH_Lamp_NeoPixel : public SH_Lamp {
private:
  uint8_t pin_;
  uint16_t len_;
  Adafruit_NeoPixel led_stripe_;

  bool setLEDColor(uint8_t, uint8_t, uint8_t);

public:
  explicit SH_Lamp_NeoPixel(const JsonObject&);

  void refresh() override;
};
