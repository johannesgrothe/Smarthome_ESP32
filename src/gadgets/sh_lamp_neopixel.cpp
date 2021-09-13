#include "sh_lamp_neopixel.h"

#include <utility>

bool SH_Lamp_NeoPixel::setLEDColor(uint8_t r, uint8_t g, uint8_t b) {
  logger_i(getName(), "Setting Color: (%d, %d, %d)", r, g, b);
  pauseAllTasks();
  for (uint16_t k = 0; k < len_; k++) {
    led_stripe_.setPixelColor(k, Adafruit_NeoPixel::Color(r, g, b));
  }
  led_stripe_.show();
  resumeTasks();
  return true;
}

SH_Lamp_NeoPixel::SH_Lamp_NeoPixel(std::string name, SHLampType type, uint8_t pin, uint16_t len) :
  SH_Lamp(std::move(name), type),
  pin_(pin),
  len_(len) {
  led_stripe_ = Adafruit_NeoPixel(len_, pin_, NEO_GRB + NEO_KHZ800);
  led_stripe_.begin();
  led_stripe_.clear();
  setColor(0, 0, 0);
}
