#include "gadget_collection.h"

bool Gadget_Collection::addGadget(const shared_ptr<SH_Gadget>& new_gadget) {
  gadgets.push_back(new_gadget);
  return true;
}

shared_ptr<SH_Gadget> Gadget_Collection::getGadget(const std::string& gadget_name) {
  for (auto gadget: gadgets) {
    if (gadget->getName() == gadget_name) {
      return gadget;
    }
  }
  return nullptr;
}

shared_ptr<SH_Gadget> Gadget_Collection::getGadget(byte index) const {
  if (index >= gadgets.size())
    return nullptr;
  return gadgets[index];
}

shared_ptr<SH_Gadget> Gadget_Collection::operator[](int index) const {
  if (index >= gadgets.size() || index < 0)
    return nullptr;
  return getGadget(index);
}

byte Gadget_Collection::getGadgetCount() const {
  return gadgets.size();
}