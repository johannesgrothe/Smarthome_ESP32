#pragma once

#include "../abstract/lamp_onoff.h"

class Lamp_Basic final : public Lamp_OnOff {
    uint8_t pin_;
    bool status_;

protected:
    gadget_definitions::GadgetIdentifier encodeType() override;

public:
    explicit Lamp_Basic(std::string name, uint8_t pin);

    void refresh() override;
};
