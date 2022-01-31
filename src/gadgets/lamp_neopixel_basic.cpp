#include "lamp_neopixel_basic.h"

#include <utility>

Lamp_NeoPixel_RGB_Basic::Lamp_NeoPixel_RGB_Basic(std::string name, uint8_t pin, uint16_t len) :
    Lamp_NeoPixel_RGB(std::move(name), pin, len) {
  setLEDColor(0xFF, 0xFF, 0xFF);
}

void Lamp_NeoPixel_RGB_Basic::refresh() {
  if (hasChanged()) {
    if (!getCharacteristicValue(gadget_definitions::CharacteristicIdentifier::status)) {
      setLEDColor(0, 0, 0);
      return;
    }
    auto hue = getCharacteristicValue(gadget_definitions::CharacteristicIdentifier::hue);
    auto saturation = getCharacteristicValue(gadget_definitions::CharacteristicIdentifier::saturation);
    uint8_t lightness = 50;
    auto brightness = getCharacteristicValue(gadget_definitions::CharacteristicIdentifier::brightness);
    logger_i(TAG, "Hue: %d; Saturation: %d; Lightness: %d; Brightness: %d", hue, saturation, lightness, brightness);
    Color buf_clr;
    buf_clr.setHSL(hue,
                   saturation,
                   lightness);
    auto rgb_color = buf_clr.getRGB();
    uint8_t r = rgb_color->getRed();
    uint8_t g = rgb_color->getGreen();
    uint8_t b = rgb_color->getBlue();

    r = ((r * brightness) / 100);
    g = ((g * brightness) / 100);
    b = ((b * brightness) / 100);

    setLEDColor(r, g, b);
  }
}
