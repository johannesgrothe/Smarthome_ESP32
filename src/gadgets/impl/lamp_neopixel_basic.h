#pragma once

#include "../abstract/lamp_neopixel.h"
#include <Arduino.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

class Lamp_NeoPixel_RGB_Basic final : public Lamp_NeoPixel_RGB {
protected:
    gadget_definitions::GadgetIdentifier encodeType() override;

public:
    explicit Lamp_NeoPixel_RGB_Basic(std::string name, uint8_t pin, uint16_t len);

    void refresh() override;
};
