#include "client_main.h"

#include "storage/static_storage.h"
#include "storage/eeprom_storage.h"
#include "gadget_factory.h"

#include "hardware_controller.h"
#include "network_loader.h"
#include "random.h"
#include "api/gadget_meta_coder.h"
#include "system/api_definitions.h"
#include "power_management/battery_types/aaa_alkaline.h"
#include "power_management/grid_manager.h"

static const char *TAG = "Initialization";

ClientMain::ClientMain(BootMode boot_mode, const SystemConfig &system_config, const GadgetConfig &gadget_config,
                       const EventConfig &event_config) :
    ApiManagerDelegate(),
    system_mode_(boot_mode),
    system_storage_(nullptr),
    power_manager_(nullptr),
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

      initEventMapping(event_config);
      initPowerManager();
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

  initApi(system_config.id);
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
    ir_gadget_ = std::make_shared<IR_Gadget>(ir_recv, ir_send);
  } else {
    logger_i("System", "No IR Configured");
    ir_gadget_ = nullptr;
  }

  logger_i("System", "Creating Radio-Gadget:");

  if (radio_recv || radio_send) {
    logger_i("System", "Radio Configured bot not implemented");
  } else {
    logger_i("System", "No Radio Configured");
    radio_gadget_ = nullptr;
  }

  return true;
}

bool ClientMain::initGadgets(const GadgetConfig &config) {
  logger_i("System", "Initializing gadget manager");
  gadget_manager_ = std::make_shared<GadgetManager>();

  auto eeprom_gadgets = config.gadgets;

  auto factory = GadgetFactory(ir_gadget_, radio_gadget_);

  logger_i("System", "Initializing Gadgets: %d", eeprom_gadgets.size());

  for (auto gadget: eeprom_gadgets) {
    auto gadget_ident = (gadget_definitions::GadgetIdentifier) std::get<0>(gadget);
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

bool ClientMain::initApi(const std::string& client_id) {
  uint16_t runtime_id = random_int(10000);

  logger_i("System", "Runtime ID: %d", runtime_id);
  logger_i("System", "Client ID: '%s'", client_id.c_str());

  api_manager_ = std::make_shared<ApiManager>(this, network_, runtime_id, client_id);
  scheduled_messages_ = std::make_shared<ScheduledMessagesManager>(api_manager_);
  return true;
}

bool ClientMain::initPowerManager() {
  logger_i("System", "Initializing power manager");
  if (PWR_TYPE == 1) {
    power_manager_ = std::make_shared<AAA_Alkaline>(PWR_PIN, PWR_CELL_COUNT);
  } else if (PWR_TYPE == 0) {
    power_manager_ = std::make_shared<GridManager>();
  } else {
    return false;
  }
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

bool ClientMain::handleGadgetConfigWrite(GadgetConfig cfg) {
  if (system_storage_ == nullptr) {
    logger_e(TAG, "Cannot write gadget config because of missing storage manager");
    return false;
  }
  return system_storage_->saveGadgetConfig(cfg);
}

bool ClientMain::handleEventConfigWrite(EventConfig cfg) {
  if (system_storage_ == nullptr) {
    logger_e(TAG, "Cannot write event config because of missing storage manager");
    return false;
  }
  return system_storage_->saveEventConfig(cfg);
}

ClientMeta ClientMain::getClientData() {
  return {getPortMapping(),
          system_mode_,
          getSoftwareFlashDate(),
          getSoftwareGitCommit(),
          getSoftwareGitBranch(),
          api_definitions::version::major,
          api_definitions::version::minor,
          api_definitions::version::bugfix};
}

std::vector<GadgetMeta> ClientMain::getGadgetData() {
  std::vector<GadgetMeta> gadget_data;
  if (gadget_manager_ == nullptr) {
    return gadget_data;
  }
  for (uint8_t i = 0; i < gadget_manager_->getGadgetCount(); i++) {
    auto buf_gadget = gadget_manager_->getGadget(i);
    std::vector<CharacteristicMeta> characteristics;
    for (auto c: buf_gadget->getCharacteristics()) {
      auto buf_c = CharacteristicMeta(c.type,
                                      c.min,
                                      c.max,
                                      c.getStepValue(),
                                      c.steps);
      characteristics.push_back(buf_c);
    }
    auto buf_g = GadgetMeta(gadget_definitions::GadgetIdentifier::lamp_neopixel_rgb_basic,  // TODO: Get actual gadget type from somewhere
                            buf_gadget->getName(),
                            characteristics);
    gadget_data.push_back(buf_g);
  }
  return gadget_data;
}

void ClientMain::setStorageManager(const std::shared_ptr<SystemStorage> &storage) {
  system_storage_ = storage;
}

void ClientMain::loopSystem() {
  if (network_->hasRequest()) {
    auto req = network_->getRequest();
    api_manager_->handleRequest(req);
  }

  scheduled_messages_->loop();

  if (ir_gadget_ != nullptr) {
    ir_gadget_->refresh();
    if (ir_gadget_->hasNewCommand()) {
      auto command = ir_gadget_->getCommand();
      event_manager_->handleCode(command->getCode());
    }
  }

  if (radio_gadget_ != nullptr) {
    radio_gadget_->refresh();
    if (radio_gadget_->hasNewCommand()) {
      auto command = radio_gadget_->getCommand();
      event_manager_->handleCode(command->getCode());
    }
  }

  if (event_manager_ != nullptr) {
    if (event_manager_->hasEvent()) {
      auto event = event_manager_->getEvent();
      gadget_manager_->forwardEvent(event);
    }
  }

  if (power_manager_ != nullptr) {
    if (power_manager_->refresh()) {
      auto power_mode = power_manager_->getPowerMode();
      if (power_mode == PowerMode::Battery_Mode) {
        auto* battery_manager = static_cast<BatteryManager *>(power_manager_.get());
        auto battery_level = battery_manager->getBatteryLevel();
        api_manager_->publishClientUpdate(battery_level);
        logger_i("System", "remaining battery charge: %d", battery_level);
      } else {
        api_manager_->publishClientUpdate(42); // TODO: remove this at some point
        logger_e("System", "tha fuck?");
      }
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
