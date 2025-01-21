#include "lamp_neopixel_basic.h"

#include <utility>

Lamp_NeoPixel_RGB_Basic::Lamp_NeoPixel_RGB_Basic(std::string name, uint8_t pin, uint16_t len) : Lamp_NeoPixel_RGB(
    std::move(name), pin, len) {
    setLedColor(0xFF, 0xFF, 0xFF);
}

void Lamp_NeoPixel_RGB_Basic::refresh() {
    if (wasChanged()) {
        setLedColor(red_, green_, blue_);
    }
}
