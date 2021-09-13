#pragma once

#include "sh_lamp_neopixel.h"
#include <Arduino.h>
#include "Adafruit_NeoPixel.h"

#ifdef __AVR__
#include <avr/power.h>
#endif

class SH_Lamp_NeoPixel_Basic : public SH_Lamp_NeoPixel {
public:

  explicit SH_Lamp_NeoPixel_Basic(std::string name, uint8_t pin, uint16_t len);

  void refresh() override;
};

static std::shared_ptr<SH_Lamp_NeoPixel_Basic> createSHLampNeoPixelBasic(std::string name, pin_set pins, const JsonObject& gadget_data) {
  uint8_t pin = 0;
  uint16_t len = 0;

  if (pins[0] != 0) {
    pin = pins[0];
  } else {
    logger_e("LampNeopixelBasic", "No pin set");
    return nullptr;
  }

  if (gadget_data.containsKey("length")) {
    len = gadget_data["length"].as<uint16_t>();
  } else {
    logger_e("LampNeopixelBasic", "No length specified");
    return nullptr;
  }

  logger_i("LampNeopixelBasic", "Pin: %d", pin);
  logger_i("LampNeopixelBasic", "Length: %d", len);

  return std::make_shared<SH_Lamp_NeoPixel_Basic>(name, pin, len);
}