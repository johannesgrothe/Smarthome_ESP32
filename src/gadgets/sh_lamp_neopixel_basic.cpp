#include "sh_lamp_neopixel_basic.h"

#include <utility>

SH_Lamp_NeoPixel_Basic::SH_Lamp_NeoPixel_Basic(std::string name, uint8_t pin, uint16_t len) :
  SH_Lamp_NeoPixel(std::move(name), SHLampType::CLR_BRI, pin, len) {
    setColor(0, 0, 0xFF);
    delay(500);
    setColor(0, 0xFF, 0);
    delay(500);
    setColor(0xFF, 0, 0);
    delay(500);
    setLEDColor(0, 0, 0);
}

void SH_Lamp_NeoPixel_Basic::refresh() {
  if (gadgetHasChanged()) {
    logger.println(getName(), "has changed.");
    uint8_t rgb[3];
    getColor(&rgb[0]);
    setLEDColor(rgb[SH_CLR_red], rgb[SH_CLR_green], rgb[SH_CLR_blue]);
  }
}
