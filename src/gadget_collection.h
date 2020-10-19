#pragma once

#include <memory>
#include "gadgets/sh_gadget.h"
#include "system_settings.h"

class Gadget_Collection {
private:

  std::vector<shared_ptr<SH_Gadget>> gadgets;

public:

  bool addGadget(const shared_ptr<SH_Gadget>& new_gadget);

  shared_ptr<SH_Gadget> getGadget(const std::string& gadget_name);

  shared_ptr<SH_Gadget> getGadget(uint8_t index) const;

  shared_ptr<SH_Gadget> operator[](int index) const;

  byte getGadgetCount() const;
};
