#include "lamp_neopixel.h"

#include <utility>

bool Lamp_NeoPixel_RGB::setLEDColor(uint8_t r, uint8_t g, uint8_t b) {
  logger_i(getName(), "Setting Color: (%d, %d, %d)", r, g, b);
  for (uint16_t k = 0; k < len_; k++) {
    led_stripe_.setPixelColor(k, Adafruit_NeoPixel::Color(r, g, b));
  }
  led_stripe_.show();
  return true;
}

Lamp_NeoPixel_RGB::Lamp_NeoPixel_RGB(std::string name, uint8_t pin, uint16_t len) :
    Lamp_RGB(std::move(name)),
    SimpleHardwareGadget(true),
    pin_(pin),
    len_(len) {
  led_stripe_ = Adafruit_NeoPixel(len_, pin_, NEO_GRB + NEO_KHZ800);
  led_stripe_.begin();
  led_stripe_.clear();
}

void Lamp_NeoPixel_RGB::executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) {
  setHWChangeStatus(true);
}
