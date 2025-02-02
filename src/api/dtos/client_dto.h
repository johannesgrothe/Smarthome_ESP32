#pragma once

#include <cstdint>
#include <string>
#include <boot_mode.h>
#include "system/variants.h"

class ClientDTO {
public:
    /**
     * Constructor for the client meta container
     *
     * @param boot_mode Mode the client is booted in
     * @param hw_variant Hardware-Variant of the client (exposed ports, attached connectors like ir...)
     * @param sw_variant Software-Variant of the client (gadgets usw)
     * @param sw_uploaded Timestamp of the software flashing process
     * @param sw_commit Complete commit-hash of the written software
     * @param sw_branch Branch of the commit flashed to the microcontroller
     * @param api_version_major Major version of the clients api version
     * @param api_version_minor Minor version of the clients api version
     * @param api_version_bugfix Bugfix version of the clients api version
     */
    ClientDTO(BootMode boot_mode,
               HwVariant hw_variant,
               SwVariant sw_variant,
               std::string sw_uploaded,
               std::string sw_commit,
               std::string sw_branch,
               uint8_t api_version_major,
               uint8_t api_version_minor,
               uint8_t api_version_bugfix);

    const BootMode boot_mode;
    const HwVariant hw_variant;
    const SwVariant sw_variant;
    const std::string sw_uploaded;
    const std::string sw_commit;
    const std::string sw_branch;
    const uint8_t api_version_major;
    const uint8_t api_version_minor;
    const uint8_t api_version_bugfix;
};
