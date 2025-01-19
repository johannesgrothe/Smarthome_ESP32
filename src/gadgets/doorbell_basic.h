#pragma once

#include "doorbell.h"
#define HW_BOUNCE_DELAY 50

class Doorbell_Basic : public Doorbell {
protected:
    uint8_t switch_pin_;

    bool default_state_;

    unsigned long last_event_;

public:
    explicit Doorbell_Basic(std::string name, uint8_t pin, bool default_state);

    void refresh() override;
};
