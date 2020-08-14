#include "sh_lamp_neopixel_basic.h"

SH_Lamp_NeoPixel_Basic::SH_Lamp_NeoPixel_Basic(const JsonObject& gadget) :
  SH_Lamp(gadget, SHLampType::CLR_BRI) {
  if (gadget["length"] != nullptr) {
    len_ = gadget["length"].as<uint16_t>();
    logger.print("Length: ");
    logger.addln(len_);
  } else {
    len_ = 1;
    logger.println(LOG_TYPE::ERR, "No Length selected.");
  }
  if (gadget["pin"] != nullptr) {
    pin_ = gadget["pin"].as<uint8_t>();
    logger.print("Pin: ");
    logger.addln(pin_);

//      pinMode(pin, OUTPUT);
//      led_stripe = new NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod>(len, pin);
    setColor(0, 0, 0xFF);
    delay(500);
    setColor(0, 0xFF, 0);
    delay(500);
    setColor(0xFF, 0, 0);
    delay(500);
    setLEDColor(0, 0, 0);
  } else {
    pin_ = 0;
    logger.println(LOG_TYPE::ERR, "No Pin selected.");
  }
}

void SH_Lamp_NeoPixel_Basic::refresh() {
  if (has_changed) {
//      logger.print(name, "has changed.\n");
    uint8_t rgb[3];
    getColor(&rgb[0]);
    setLEDColor(rgb[0], rgb[1], rgb[2]);

//      if (getStatus())
//        setLEDColor(0xFF, 0xFF, 0xFF);
//      else
//        setLEDColor(0, 0, 0);
  }
  has_changed = false;
}

void SH_Lamp_NeoPixel_Basic::setLEDColor(uint8_t r, uint8_t g, uint8_t b) {
  Serial.printf("[%s] Setting Color: (%d, %d, %d)\n", getName(), r, g, b);
//    RGBColor clr(r, g, b);
  for (uint16_t k = 0; k < len_; k++) {
//      led_stripe->SetPixelColor(k, clr);
  }
//    led_stripe->Show();
}