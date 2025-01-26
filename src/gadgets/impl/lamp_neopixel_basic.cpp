#include "lamp_neopixel_basic.h"

#include <utility>

gadget_definitions::GadgetIdentifier Lamp_NeoPixel_RGB_Basic::encodeType() {
    return gadget_definitions::GadgetIdentifier::lamp_neopixel_rgb_basic;
}

Lamp_NeoPixel_RGB_Basic::Lamp_NeoPixel_RGB_Basic(std::string name, const uint8_t pin, const uint16_t len) : Lamp_NeoPixel_RGB(
    std::move(name), pin, len) {
    setLedColor(0xFF, 0xFF, 0xFF);
}

void Lamp_NeoPixel_RGB_Basic::refresh() {
    if (wasChanged()) {
        setLedColor(red_, green_, blue_);
    }
}
