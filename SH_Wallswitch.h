#ifndef __SH_Wallswitch__
#define __SH_Wallswitch__

#define BOUNCE_DELAY 5
#define MULTI_KLICK_TIME 200
#define LONG_KLICK_TIME 1000

#include <Arduino.h>

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
        is_initialized(false),
        has_new_status(false),
        last_action(0),
        btn_status(false),
        klick_counter(0),
        status(0)
        {
        };

    bool hasNewStatus();

    byte getStatus();

    bool getRawStatus();

    void refresh();

    bool init(uint8_t pin, bool default_pinstate);
};

#endif
