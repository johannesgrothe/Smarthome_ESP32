#pragma once

#include <string>
#include <vector>

#include "characteristic_update_meta.h"

class GadgetUpdateMeta {
public:
  const std::string id;
  const std::vector<CharacteristicUpdateMeta> characteristics;

  GadgetUpdateMeta(std::string id, std::vector<CharacteristicUpdateMeta> characteristics);
};
