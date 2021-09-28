#pragma once


#include <cstdint>
#include <vector>
#include <tuple>
#include <string>
#include "../boot_mode.h"

class ClientMeta {
public:
  ClientMeta(uint16_t runtime_id,
             std::vector<std::tuple<int, int>> port_mapping,
             BootMode boot_mode,
             std::string sw_uploaded,
             std::string sw_commit,
             std::string sw_branch);

  const uint16_t runtime_id;
  const std::vector<std::tuple<int, int>> port_mapping;
  const BootMode boot_mode;
  const std::string sw_uploaded;
  const std::string sw_commit;
  const std::string sw_branch;
};