#pragma once

#include "lamp_onoff.h"
#include "simple_hardware_gadget.h"

class Lamp_Basic final : public Lamp_OnOff, public SimpleHardwareGadget {
    uint8_t pin_;
    bool status_;

public:
    explicit Lamp_Basic(std::string name, uint8_t pin);
    void refresh() override;
};
