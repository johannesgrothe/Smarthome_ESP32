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
   * @param port_mapping Mapping of the ports to the pins of the microcontroller
   * @param boot_mode Mode the client is booted in
   * @param sw_uploaded Timestamp of the software flashing process
   * @param sw_commit Complete commit-hash of the written software
   * @param sw_branch Branch of the commit flashed to the microcontroller
   * @param api_version_major Major version of the clients api version
   * @param api_version_minor Minor version of the clients api version
   * @param api_version_bugfix Bugfix version of the clients api version
   */
  ClientMeta(std::vector<std::tuple<int, int>> port_mapping,
             BootMode boot_mode,
             std::string sw_uploaded,
             std::string sw_commit,
             std::string sw_branch,
             uint8_t api_version_major,
             uint8_t api_version_minor,
             uint8_t api_version_bugfix);

  const std::vector<std::tuple<int, int>> port_mapping;
  const BootMode boot_mode;
  const std::string sw_uploaded;
  const std::string sw_commit;
  const std::string sw_branch;
  const uint8_t api_version_major;
  const uint8_t api_version_minor;
  const uint8_t api_version_bugfix;
};