#ifndef __gadget_collection__
#define __gadget_collection__

#include "gadgets/sh_gadget.h"
#include "system_settings.h"

class Gadget_Collection {
private:

  SH_Gadget *gadgets[MAIN_MAX_GADGETS]{};

  byte gadget_count{};

public:

  bool addGadget(SH_Gadget *new_gadget) {
    if (gadget_count >= MAIN_MAX_GADGETS - 1)
      return false;
    gadgets[gadget_count] = new_gadget;
    gadget_count += 1;
    return true;
  }

  SH_Gadget *getGadget(const char *gadget_name) {
    for (byte k = 0; k < gadget_count; k++) {
      SH_Gadget *it_gadget = gadgets[k];
      if (strcmp(it_gadget->getName(), gadget_name) == 0) {
        return it_gadget;
      }
    }
    return nullptr;
  }

  SH_Gadget *getGadget(byte index) {
    if (index > gadget_count)
      return nullptr;
    return gadgets[index];
  }

  byte getGadgetCount() {
    return gadget_count;
  }
};

#endif //__gadget_collection__