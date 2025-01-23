#include "client_main.h"

#include "storage/static_storage.h"
#include "storage/eeprom_storage.h"
#include "gadget_factory.h"

#include "hardware_controller.h"
#include "network_loader.h"
#include "random.h"
#include "api/gadget_meta_coder.h"
#include "system/api_definitions.h"

#include "config_variants.h"

static auto TAG = "Initialization";

ClientMain::ClientMain(const BootMode boot_mode, const SystemConfig &system_config) : ApiManagerDelegate(),
    system_mode_(boot_mode),
    system_storage_(nullptr),
    api_manager_(nullptr),
    gadget_manager_(nullptr),
    event_manager_(nullptr),
    network_(nullptr),
    ir_gadget_(nullptr),
    radio_gadget_(nullptr) {
    logger_i("System", "Launching...");
    logger_i("System", "Software Info:");
    logger_i("System", "Flash Date: %s", getSoftwareFlashDate().c_str());
    logger_i("System", "Git Branch: %s", getSoftwareGitBranch().c_str());
    logger_i("System", "Git Commit: %s", getSoftwareGitCommit().c_str());
    logger_i("System", "API Version: %d.%d.%d",
             api_definitions::version::major,
             api_definitions::version::minor,
             api_definitions::version::bugfix);

    bool status;

    switch (system_mode_) {
        case BootMode::Serial_Only:
            logger_i("System", "Boot Mode: Serial Only");
            status = initNetwork(system_config, NetworkMode::Serial);
            if (!status) {
                HardwareController::rebootChip("Network initialization failed.", 15);
            }
            break;
        case BootMode::Network_Only:
            logger_i("System", "Boot Mode: Network Only");
            status = initNetwork(system_config, system_config.network_mode);
            if (!status) {
                HardwareController::rebootChip("Network initialization failed.", 15);
            }
            break;
        case BootMode::Full_Operation:
            logger_i("System", "Boot Mode: Full Operation");
            status = initNetwork(system_config, system_config.network_mode);
            if (!status) {
                HardwareController::rebootChip("Network initialization failed.", 15);
            }

        // initEventMapping(event_config);
            initConnectors();
            initGadgets();

            break;
        default:
            logger_i("System", "Unknown Boot Mode");
            break;
    }

#ifndef UNIT_TEST
    logger_i("System", "Free Heap: %d", ESP.getFreeHeap());
#endif

    initApi(system_config.id);
}

bool ClientMain::initNetwork(const SystemConfig &config, NetworkMode mode) {
    const auto network = NetworkLoader::loadNetwork(config, mode);
    if (network == nullptr) {
        return false;
    }
    network_ = network;
    return true;
}

bool ClientMain::initConnectors() {
    logger_i("System", "Initializing Connectors:");

    logger_i("System", "Initializing IR-Gadget:");
    ir_gadget_ = loadIRGadget();
    if (ir_gadget_) {
        logger_i("System", "IR Initialized");
    } else {
        logger_i("System", "No IR Configured");
    }

    logger_i("System", "Creating Radio-Gadget:");

    radio_gadget_ = loadRadioGadget();
    if (ir_gadget_) {
        logger_i("System", "IR Initialized");
    } else {
        logger_i("System", "No IR Configured");
    }

    return true;
}

bool ClientMain::initGadgets() {
    logger_i("System", "Initializing gadget manager");
    gadget_manager_ = std::make_shared<GadgetManager>();

    // auto eeprom_gadgets = config.gadgets;

    auto factory = GadgetFactory(ir_gadget_, radio_gadget_);

    logger_i("System", "Initializing Gadgets: %d");

    applyGadgetConfiguration(gadget_manager_);

    return true;
}

bool ClientMain::initEventMapping(const EventConfig &config) {
    logger_i("System", "Initializing event manager");
    event_manager_ = std::make_shared<EventManager>(config.event_mapping);
    return true;
}

bool ClientMain::initApi(const std::string &client_id) {
    uint16_t runtime_id = random_int(10000);

    logger_i("System", "Runtime ID: %d", runtime_id);
    logger_i("System", "Client ID: '%s'", client_id.c_str());

    api_manager_ = std::make_shared<ApiManager>(this, network_, runtime_id, client_id);
    scheduled_messages_ = std::make_shared<ScheduledMessagesManager>(api_manager_);
    return true;
}

void ClientMain::handleGadgetUpdate(GadgetUpdateMeta gadget) {
    gadget_manager_->forwardUpdate(gadget);
}

void ClientMain::handleEvent(Event event) {
    event_manager_->handleEvent(std::make_shared<Event>(event));
}

bool ClientMain::handleSystemConfigWrite(SystemConfig cfg) {
    if (system_storage_ == nullptr) {
        logger_e(TAG, "Cannot write system config because of missing storage manager");
        return false;
    }
    return system_storage_->saveSystemConfig(cfg);
}

// bool ClientMain::handleGadgetConfigWrite(GadgetConfig cfg) {
//     if (system_storage_ == nullptr) {
//         logger_e(TAG, "Cannot write gadget config because of missing storage manager");
//         return false;
//     }
//     return system_storage_->saveGadgetConfig(cfg);
// }

// bool ClientMain::handleEventConfigWrite(EventConfig cfg) {
//     if (system_storage_ == nullptr) {
//         logger_e(TAG, "Cannot write event config because of missing storage manager");
//         return false;
//     }
//     return system_storage_->saveEventConfig(cfg);
// }

ClientMeta ClientMain::getClientData() {
    return {
        getPortMapping(),
        system_mode_,
        getSoftwareFlashDate(),
        getSoftwareGitCommit(),
        getSoftwareGitBranch(),
        api_definitions::version::major,
        api_definitions::version::minor,
        api_definitions::version::bugfix
    };
}

std::vector<GadgetMeta> ClientMain::getGadgetData() {
    std::vector<GadgetMeta> gadget_data;
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

void ClientMain::setStorageManager(const std::shared_ptr<SystemStorage> &storage) {
    system_storage_ = storage;
}

void ClientMain::loopSystem() {
    if (network_->hasRequest()) {
        const auto req = network_->getRequest();
        api_manager_->handleRequest(req);
    }

    scheduled_messages_->loop();

    if (ir_gadget_ != nullptr) {
        ir_gadget_->refresh();
        if (ir_gadget_->hasNewCommand()) {
            const auto command = ir_gadget_->getCommand();
            event_manager_->handleCode(command->getCode());
        }
    }

    if (radio_gadget_ != nullptr) {
        radio_gadget_->refresh();
        if (radio_gadget_->hasNewCommand()) {
            const auto command = radio_gadget_->getCommand();
            event_manager_->handleCode(command->getCode());
        }
    }

    if (event_manager_ != nullptr) {
        if (event_manager_->hasEvent()) {
            auto event = event_manager_->getEvent();
            gadget_manager_->forwardEvent(event);
        }
    }
}

void ClientMain::loopGadgets() {
    if (gadget_manager_ != nullptr) {
        gadget_manager_->refresh();
        for (uint8_t i = 0; i < gadget_manager_->getGadgetCount(); i++) {
            auto gadget = gadget_manager_->getGadget(i);
            if (gadget->hasChanged()) {
                auto g = GadgetMetaEncoder::encodeGadgetUpdate(gadget);
                api_manager_->publishGadgetUpdate(g);
            }
        }
    }
}

void ClientMain::loopNetwork() {
    network_->refresh();
}
