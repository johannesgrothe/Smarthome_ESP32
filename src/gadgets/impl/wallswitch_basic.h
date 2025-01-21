#pragma once

#include "../abstract/wallswitch.h"

class Switch_Basic final : public Switch {
    uint8_t pin_;
    unsigned long last_action_;
    bool default_state_;

public:
    Switch_Basic(std::string name, uint8_t pin, bool default_state);

    void refresh() override;
};
