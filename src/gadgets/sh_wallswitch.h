#pragma once

#define BOUNCE_DELAY 5
#define MULTI_KLICK_TIME 200
#define LONG_KLICK_TIME 1000

#include <Arduino.h>

//TODO: Fix this mess

class SH_Wallswitch {
private:
    uint8_t status;
    uint8_t switch_pin;
    uint8_t klick_counter;
    bool has_new_status;
    bool btn_status;
    bool def_pinstate;
    bool is_initialized;
    unsigned long last_action;

public:
    SH_Wallswitch():
        status(0),
        switch_pin(0),
        klick_counter(0),
        has_new_status(false),
        btn_status(false),
        is_initialized(false),
        last_action(0)
        {
        }

    bool hasNewStatus();

    byte getStatus() const;

    bool getRawStatus() const;

    void refresh();

    bool init(uint8_t, bool);
};
