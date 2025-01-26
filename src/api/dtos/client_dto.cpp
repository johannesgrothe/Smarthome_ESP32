#include "client_dto.h"

#include <boot_mode.h>
#include <utility>
#include <vector>

ClientDTO::ClientDTO(const BootMode boot_mode,
                     const HwVariant hw_variant,
                     const SwVariant sw_variant,
                     std::string sw_uploaded,
                     std::string sw_commit,
                     std::string sw_branch,
                     const uint8_t api_version_major,
                     const uint8_t api_version_minor,
                     const uint8_t api_version_bugfix) : boot_mode(boot_mode),
                                                         hw_variant(hw_variant),
                                                         sw_variant(sw_variant),
                                                         sw_uploaded(std::move(sw_uploaded)),
                                                         sw_commit(std::move(sw_commit)),
                                                         sw_branch(std::move(sw_branch)),
                                                         api_version_major(api_version_major),
                                                         api_version_minor(api_version_minor),
                                                         api_version_bugfix(api_version_bugfix) {
}
