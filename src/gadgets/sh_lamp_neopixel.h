#ifndef __SH_Lamp_NeoPixel__
#define __SH_Lamp_NeoPixel__

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
  explicit SH_Lamp_NeoPixel(JsonObject);

  void refresh() override;
};

#endif