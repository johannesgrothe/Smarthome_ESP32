#pragma once

#include <string>
#include <vector>

#include "../system/gadget_definitions.h"
#include "../gadgets/gadget.h"
#include "../datatypes.h"
#include "characteristic_meta.h"

class GadgetMeta {
public:
  const gadget_definitions::GadgetIdentifier type;
  const std::string name;
  const std::vector<CharacteristicMeta> characteristics;

  GadgetMeta(gadget_definitions::GadgetIdentifier type, std::string name, std::vector<CharacteristicMeta> characteristics);
};
