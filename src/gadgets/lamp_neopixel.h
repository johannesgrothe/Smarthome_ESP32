#pragma once

#include "lamp_rgb.h"
#include "simple_hardware_gadget.h"
#include <Adafruit_NeoPixel.h>

class Lamp_NeoPixel_RGB : public Lamp_RGB, public SimpleHardwareGadget {
private:
  uint8_t pin_;
  uint16_t len_;
  Adafruit_NeoPixel led_stripe_;

protected:
  bool setLEDColor(uint8_t, uint8_t, uint8_t);

  void executeCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier characteristic, uint16_t step_value) override;

public:
  Lamp_NeoPixel_RGB(std::string name, uint8_t pin, uint16_t len);

};
