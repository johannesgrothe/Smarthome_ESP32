#pragma once

#include <string>

#include "../gadgets/gadget_enums.h"
#include "../datatypes.h"
#include "characteristic_meta.h"

class GadgetMeta {
public:
  GadgetMeta(GadgetIdentifier type, std::string name, std::vector<CharacteristicMeta> characteristics);

  const GadgetIdentifier type;
  const std::string name;
  const std::vector<CharacteristicMeta> characteristics;
};
