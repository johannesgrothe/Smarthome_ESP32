#include "gadget_collection.h"

bool Gadget_Collection::addGadget(SH_Gadget *new_gadget) {
  if (gadget_count >= MAIN_MAX_GADGETS - 1)
    return false;
  gadgets[gadget_count] = new_gadget;
  gadget_count += 1;
  return true;
}

SH_Gadget *Gadget_Collection::getGadget(const char *gadget_name) {
  for (byte k = 0; k < gadget_count; k++) {
    SH_Gadget *it_gadget = gadgets[k];
    if (strcmp(it_gadget->getName(), gadget_name) == 0) {
      return it_gadget;
    }
  }
  return nullptr;
}

SH_Gadget *Gadget_Collection::getGadget(byte index) const {
  if (index > gadget_count)
    return nullptr;
  return gadgets[index];
}

SH_Gadget *Gadget_Collection::operator[](int index) const {
  return getGadget(index);
}

byte Gadget_Collection::getGadgetCount() const {
  return gadget_count;
}