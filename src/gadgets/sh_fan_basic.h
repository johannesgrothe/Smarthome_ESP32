#ifndef __SH_FAN_BASIC__
#define __SH_FAN_BASIC__

#include "sh_fan.h"

class SH_Fan_Basic : public SH_Fan {
protected:
  bool status;
  byte rotation_speed;
  byte last_rotation_speed;

  unsigned long code_lvl_0;
  unsigned long code_lvl_1;
  unsigned long code_lvl_2;
  unsigned long code_lvl_3;

public:

  explicit SH_Fan_Basic(JsonObject gadget) :
    SH_Fan(gadget) {

  };

  void refresh() override {
    if (has_changed) {
      logger.println(name, "has changed.");
    }
    has_changed = false;
  }
};

#endif //__SH_FAN_BASIC__