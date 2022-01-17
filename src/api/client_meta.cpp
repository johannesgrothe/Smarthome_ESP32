#include "client_meta.h"

#include <utility>

ClientMeta::ClientMeta(std::vector<std::tuple<int, int>> port_mapping,
                       BootMode boot_mode,
                       std::string sw_uploaded,
                       std::string sw_commit,
                       std::string sw_branch, uint8_t api_version_major,
                       uint8_t api_version_minor, uint8_t api_version_bugfix) :
    port_mapping(std::move(port_mapping)),
    boot_mode(boot_mode),
    sw_uploaded(std::move(sw_uploaded)),
    sw_commit(std::move(sw_commit)),
    sw_branch(std::move(sw_branch)),
    api_version_major(api_version_major),
    api_version_minor(api_version_minor),
    api_version_bugfix(api_version_bugfix) {}
