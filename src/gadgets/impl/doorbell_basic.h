#pragma once


#define HW_BOUNCE_DELAY 50
#include "../abstract/doorbell.h"

class Doorbell_Basic final : public Doorbell {
protected:
    uint8_t switch_pin_;

    bool default_state_;

    unsigned long last_event_;

public:
    explicit Doorbell_Basic(const std::string& name, uint8_t pin, bool default_state);

    void refresh() override;
};
