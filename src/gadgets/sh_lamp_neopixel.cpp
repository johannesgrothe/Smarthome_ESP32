#include "sh_lamp_neopixel.h"

bool SH_Lamp_NeoPixel::setLEDColor(uint8_t r, uint8_t g, uint8_t b) {
  Serial.printf("[%s] Setting Color: (%d, %d, %d)\n", getName(), r, g, b);
  for (uint16_t k = 0; k < len_; k++) {
    led_stripe_.setPixelColor(k, Adafruit_NeoPixel::Color(r, g, b));
  }
  led_stripe_.show();
  return true;
}

SH_Lamp_NeoPixel::SH_Lamp_NeoPixel(const JsonObject& gadget) :
  SH_Lamp(gadget) {
  if (gadget["pin"] != nullptr) {
    pin_ = gadget["pin"].as<uint8_t>();
  } else {
    pin_ = 0;
  }
  if (gadget["length"] != nullptr) {
    len_ = gadget["length"].as<uint16_t>();
  } else {
    len_ = 1;
  }
  led_stripe_ = Adafruit_NeoPixel(len_, pin_, NEO_GRB + NEO_KHZ800);
  led_stripe_.begin();
  led_stripe_.clear();
  setColor(0, 0, 0xFF);
}

void SH_Lamp_NeoPixel::refresh() {
  if (has_changed) {
    Serial.printf("[%s] has changed.\n", getName());
    print();
    uint8_t rgb[3];
    getColor(&rgb[0]);
    setColor(rgb[SH_CLR_red], rgb[SH_CLR_green], rgb[SH_CLR_blue]);
    print();
  }
  has_changed = false;
}