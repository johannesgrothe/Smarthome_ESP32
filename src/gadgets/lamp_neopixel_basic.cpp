#include "lamp_neopixel_basic.h"

#include <utility>

Lamp_NeoPixel_RGB_Basic::Lamp_NeoPixel_RGB_Basic(std::string name, uint8_t pin, uint16_t len) :
    Lamp_NeoPixel_RGB(std::move(name), pin, len) {
  setLEDColor(0xFF, 0xFF, 0xFF);
}

void Lamp_NeoPixel_RGB_Basic::refresh() {
  if (hasChanged()) {
    Color buf_clr;
    buf_clr.setHSV(getCharacteristicValue(CharacteristicIdentifier::hue),
                   getCharacteristicValue(CharacteristicIdentifier::saturation),
                   getCharacteristicValue(CharacteristicIdentifier::brightness));
    auto rgb_color = buf_clr.getRGB();
    setLEDColor(rgb_color->getRed(), rgb_color->getGreen(), rgb_color->getBlue());
  }
}
