#include "sh_wallswitch.h"

bool SH_Wallswitch::hasNewStatus() {
    bool buf = has_new_status;
    has_new_status = false;
    return buf;
}

void SH_Wallswitch::refresh()
{
  unsigned long local_time = millis();

  if (last_action + BOUNCE_DELAY < local_time)
  {
    if (digitalRead(switch_pin) != def_pinstate)
    {
      if (btn_status == false)
      {
        {
          // Serial.println("Klick");
          last_action = local_time;
          klick_counter ++;
          btn_status = true;
        }
      }
    }
    else
    {
      if (btn_status == true)
      {
        // Serial.println("No Klick");
        status = klick_counter;
        has_new_status = true;
        klick_counter = 0;
        btn_status = false;
      }
    }
  }
}

byte SH_Wallswitch::getStatus() const {
    return status;
}

bool SH_Wallswitch::getRawStatus() const {
    return digitalRead(switch_pin);
//    return btn_status;
}

bool SH_Wallswitch::init(uint8_t pin, bool default_pinstate) {
    switch_pin = pin;
    def_pinstate = default_pinstate;
    pinMode(switch_pin, default_pinstate);
    return true;
}
