#pragma once

#include <memory>
#include <vector>

#include "gadgets/abstract/gadget.h"
#include "api/dtos/gadget_update_dto.h"

class GadgetManager {

  std::vector<std::shared_ptr<Gadget>> gadgets_;

public:
  GadgetManager();

  bool addGadget(const std::shared_ptr<Gadget>& new_gadget);

  std::shared_ptr<Gadget> getGadget(const std::string& gadget_name);

  std::shared_ptr<Gadget> getGadget(uint8_t index) const;

  std::shared_ptr<Gadget> operator[](int index) const;

  uint8_t getGadgetCount() const;

  void forwardUpdate(const GadgetUpdateDTO &data);

  void loop();
};
