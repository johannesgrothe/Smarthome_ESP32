#pragma once

#include <memory>
#include "gadgets/sh_gadget.h"
#include "system_settings.h"

class Gadget_Collection {
private:

  std::vector<std::shared_ptr<SH_Gadget>> gadgets;

public:

  bool addGadget(const std::shared_ptr<SH_Gadget>& new_gadget);

  std::shared_ptr<SH_Gadget> getGadget(const std::string& gadget_name);

  std::shared_ptr<SH_Gadget> getGadget(uint8_t index) const;

  std::shared_ptr<SH_Gadget> operator[](int index) const;

  uint8_t getGadgetCount() const;
};
