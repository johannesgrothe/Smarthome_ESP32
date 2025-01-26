#include "client_manager.h"

#include <static_info.h>
#include <utility>

#include "storage/static_storage.h"
#include "storage/eeprom_storage.h"

#include "network_loader.h"
#include "random.h"
#include "system/api_definitions.h"

#include "config_variants.h"

static auto TAG = "Initialization";

ClientManager::ClientManager(const BootMode boot_mode,
                             const HwVariant hw_variant,
                             std::string hw_serial,
                             const SwVariant sw_variant,
                             std::shared_ptr<SystemStorage> system_storage,
                             std::shared_ptr<GadgetManager> gadget_manager,
                             std::shared_ptr<EventManager> event_manager): system_mode_(boot_mode),
                                                                           sw_variant_(sw_variant),
                                                                           hw_variant_(hw_variant),
                                                                           hw_serial_(std::move(hw_serial)),
                                                                           system_storage_(std::move(system_storage)),
                                                                           gadget_manager_(std::move(gadget_manager)),
                                                                           event_manager_(std::move(event_manager)) {
}

void ClientManager::handleGadgetUpdate(GadgetUpdateDTO gadget) {
    gadget_manager_->forwardUpdate(gadget);
}

void ClientManager::handleEvent(Event event) {
    event_manager_->handleEvent(std::make_shared<Event>(event));
}

bool ClientManager::handleSystemConfigWrite(SystemConfig cfg) {
    if (system_storage_ == nullptr) {
        logger_e(TAG, "Cannot write system config because of missing storage manager");
        return false;
    }
    return system_storage_->saveSystemConfig(cfg);
}

ClientDTO ClientManager::getClientData() {
    return {
        system_mode_,
        hw_variant_,
        sw_variant_,
        getSoftwareFlashDate(),
        getSoftwareGitCommit(),
        getSoftwareGitBranch(),
        api_definitions::version::major,
        api_definitions::version::minor,
        api_definitions::version::bugfix
    };
}

std::vector<GadgetDTO> ClientManager::getGadgetData() {
    std::vector<GadgetDTO> gadget_data;
    if (gadget_manager_ == nullptr) {
        return gadget_data;
    }
    for (uint8_t i = 0; i < gadget_manager_->getGadgetCount(); i++) {
        const auto buf_gadget = gadget_manager_->getGadget(i);
        auto buf_g = buf_gadget->encode();
        gadget_data.push_back(buf_g);
    }
    return gadget_data;
}
