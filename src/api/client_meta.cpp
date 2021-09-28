#include "client_meta.h"

#include <utility>

ClientMeta::ClientMeta(uint16_t runtime_id, std::vector<std::tuple<int, int>> port_mapping, BootMode boot_mode,
                       std::string sw_uploaded, std::string sw_commit, std::string sw_branch) :
    runtime_id(runtime_id),
    port_mapping(std::move(port_mapping)),
    boot_mode(boot_mode),
    sw_uploaded(std::move(sw_uploaded)),
    sw_commit(std::move(sw_commit)),
    sw_branch(std::move(sw_branch)) {}
