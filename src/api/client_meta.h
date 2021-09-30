#pragma once


#include <cstdint>
#include <vector>
#include <tuple>
#include <string>
#include "../boot_mode.h"

class ClientMeta {
public:

  /**
   * Constructor for the client meta container
   *
   * @param runtime_id Runtime id of the client
   * @param port_mapping Mapping of the ports to the pins of the microcontroller
   * @param boot_mode Mode the client is booted in
   * @param sw_uploaded Timestamp of the software flashing process
   * @param sw_commit Complete commit-hash of the written software
   * @param sw_branch Branch of the commit flashed to the microcontroller
   */
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