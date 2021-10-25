#include "client_main.h"

#include "storage/static_storage.h"
#include "storage/eeprom_storage.h"
#include "gadget_factory.h"

#include "hardware_controller.h"
#include "network_loader.h"
#include "random.h"

ClientMain::ClientMain(BootMode boot_mode, const SystemConfig &system_config, const GadgetConfig &gadget_config,
                       const EventConfig &event_config) :
    ApiManagerDelegate(),
    runtime_id_(),
    system_mode_(boot_mode),
    system_storage_(nullptr),
    api_manager_(nullptr),
    gadget_manager_(nullptr),
    event_manager_(nullptr),
    network_(nullptr),
    ir_gadget(nullptr),
    radio_gadget(nullptr) {
  logger_i("System", "Launching...");

  runtime_id_ = random_int(10000);
  logger_i("System", "Runtime ID: %d", runtime_id_);

  logger_i("System", "Software Info:");
  logger_i("System", "Flash Date: %s", getSoftwareFlashDate().c_str());
  logger_i("System", "Git Branch: %s", getSoftwareGitBranch().c_str());
  logger_i("System", "Git Commit: %s", getSoftwareGitCommit().c_str());

  client_id_ = system_config.id;
  logger_i("System", "Client ID: '%s'", client_id_.c_str());

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

      initConnectors(system_config);
      initGadgets(gadget_config);

      break;
    default:
      logger_i("System", "Unknown Boot Mode");
      break;
  }

  #ifndef UNIT_TEST
  logger_i("System", "Free Heap: %d", ESP.getFreeHeap());
  #endif

  initApi();
}

bool ClientMain::initNetwork(const SystemConfig &config, NetworkMode mode) {
  auto network = NetworkLoader::loadNetwork(config, mode);
  if (network == nullptr) {
    return false;
  }
  network_ = network;
  return true;
}

bool ClientMain::initConnectors(const SystemConfig &config) {
  logger_i("System", "Initializing Connectors:");

  uint8_t ir_recv = config.ir_recv_pin;
  uint8_t ir_send = config.ir_send_pin;
  uint8_t radio_recv = config.radio_recv_pin;
  uint8_t radio_send = config.radio_send_pin;

  logger_i("System", "Creating IR-Gadget:");

  if (ir_recv || ir_send) {
    ir_gadget = std::make_shared<IR_Gadget>(ir_recv, ir_send);
  } else {
    logger_i("System", "No IR Configured");
    ir_gadget = nullptr;
  }

  logger_i("System", "Creating Radio-Gadget:");

  if (radio_recv || radio_send) {
    logger_i("System", "Radio Configured bot not implemented");
  } else {
    logger_i("System", "No Radio Configured");
    radio_gadget = nullptr;
  }

  return true;
}

bool ClientMain::initGadgets(const GadgetConfig &config) {
  logger_i("System", "Initializing gadget manager");
  gadget_manager_ = std::make_shared<GadgetManager>();

  auto eeprom_gadgets = config.gadgets;

  auto factory = GadgetFactory(ir_gadget, radio_gadget);

  logger_i("System", "Initializing Gadgets: %d", eeprom_gadgets.size());

  for (auto gadget: eeprom_gadgets) {
    auto gadget_ident = (GadgetIdentifier) std::get<0>(gadget);
//    auto remote_bf = std::get<1>(gadget);
    auto ports = std::get<2>(gadget);
    auto name = std::get<3>(gadget);
    auto gadget_config_str = std::get<4>(gadget);
    auto event_map = std::get<5>(gadget);

    logger_i("System", "Initializing %s", name.c_str());

    // Translate stored ports to actual pins
    port_set pins;
    for (int i = 0; i < pins.size(); i++) {
      uint8_t buf_port = ports[i];
      uint8_t pin = 0;
      if (buf_port != 0) {
        pin = getPinForPort(buf_port);
      }
      pins[i] = pin;
    }

    DynamicJsonDocument gadget_config(2500);
    DynamicJsonDocument code_config(2500);

    bool deserialization_ok = true;

    DeserializationError err;

    if (!gadget_config_str.empty()) {
      err = deserializeJson(gadget_config, gadget_config_str);
      if (err != DeserializationError::Ok) {
        deserialization_ok = false;
      }
    }

    if (deserialization_ok) {
      logger_i("System", "Creating Gadget");
      auto buf_gadget = factory.createGadget(gadget_ident,
                                             pins,
                                             name,
                                             gadget_config.as<JsonObject>(),
                                             event_map);

      if (buf_gadget != nullptr) {
        gadget_manager_->addGadget(buf_gadget);
      } else {
        logger_e("System", "Gadget could not be created");
      }

    } else {
      logger_e("System", "Error in config deserialization process");
    }
  }

  return true;
}

bool ClientMain::initEventMapping(const EventConfig &config) {
  logger_i("System", "Initializing event manager");
  event_manager_ = std::make_shared<EventManager>(config.event_mapping);
  return true;
}

bool ClientMain::initApi() {
  api_manager_ = std::make_shared<ApiManager>(this, network_);
  return true;
}

void ClientMain::handleGadgetUpdate(GadgetMeta gadget) {

}

void ClientMain::handleEvent(Event event) {
  event_manager_->handleEvent(std::make_shared<Event>(event));
}

std::string ClientMain::getClientId() {
  return client_id_;
}

ClientMeta ClientMain::getClientData() {
  return {runtime_id_,
          {},
          system_mode_,
          getSoftwareFlashDate(),
          getSoftwareGitCommit(),
          getSoftwareGitBranch()};
}

std::vector<GadgetMeta> ClientMain::getGadgetData() {
  return {};
}

void ClientMain::setStorageManager(const std::shared_ptr<SystemStorage> &storage) {
  system_storage_ = storage;
}

void ClientMain::loopSystem() {
  if (network_->hasRequest()) {
    auto req = network_->getRequest();
    api_manager_->handleRequest(req);
  }
}

void ClientMain::loopGadgets() {
  if (gadget_manager_ != nullptr) {
    gadget_manager_->refresh();
  }
}

void ClientMain::loopNetwork() {
  network_->refresh();
}
