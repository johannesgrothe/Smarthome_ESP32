#include "lamp_neopixel_basic.h"

#include <utility>

Lamp_NeoPixel_RGB_Basic::Lamp_NeoPixel_RGB_Basic(std::string name, uint8_t pin, uint16_t len) :
    Lamp_NeoPixel_RGB(std::move(name), pin, len) {
//    setColor(0, 0, 0xFF);
//    delay(500);
//    setColor(0, 0xFF, 0);
//    delay(500);
//    setColor(0xFF, 0, 0);
//    delay(500);
//    setLEDColor(0, 0, 0);
}

void Lamp_NeoPixel_RGB_Basic::refresh() {
  if (hasChanged()) {
    logger_i(getName(), "Has changed.");
    uint8_t rgb[3];
//    getColor(&rgb[0]);
    setLEDColor(rgb[SH_CLR_red], rgb[SH_CLR_green], rgb[SH_CLR_blue]);
  }
}
