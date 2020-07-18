#pragma once

#include "sh_lamp.h"
#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

#ifdef __AVR__
#include <avr/power.h>
#endif

class SH_Lamp_NeoPixel_Basic : public SH_Lamp {
private:
  uint8_t pin_;
  uint16_t len_;

public:

  explicit SH_Lamp_NeoPixel_Basic(const JsonObject&);

  void refresh() override;

  void setLEDColor(uint8_t, uint8_t, uint8_t);
};
