#include "client_main.h"

#include "storage/static_storage.h"
#include "storage/eeprom_storage.h"
#include "gadget_factory.h"

#include "hardware_controller.h"
#include "network_loader.h"
#include "random.h"

ClientMain::ClientMain(BootMode boot_mode, const SystemConfig& system_config, const GadgetConfig& gadget_config, const EventConfig& event_config) :
    ApiManagerDelegate(),
    runtime_id_(),
    system_mode_(boot_mode),
    system_storage_(nullptr),
    api_manager_(nullptr),
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
  auto eeprom_gadgets = config.gadgets;

  logger_i("System", "Initializing Gadgets: %d", eeprom_gadgets.size());

  for (auto gadget: eeprom_gadgets) {
    auto gadget_ident = (GadgetIdentifier) std::get<0>(gadget);
    auto remote_bf = std::get<1>(gadget);
    auto ports = std::get<2>(gadget);
    auto name = std::get<3>(gadget);
    auto gadget_config_str = std::get<4>(gadget);
    auto code_config_str = std::get<5>(gadget);

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

    if (!gadget_config_str.empty()) {
      err = deserializeJson(code_config, code_config_str);
      if (err != DeserializationError::Ok) {
        deserialization_ok = false;
      }
    }

    if (deserialization_ok) {
      #ifndef UNIT_TEST
      logger_i("System", "Creating Gadget");
      auto buf_gadget = GadgetFactory::createGadget(gadget_ident, pins, name, gadget_config.as<JsonObject>());

      if (buf_gadget != nullptr) {
        // Gadget Remote
        if (remote_bf[0]) {
          logger_i("System", "Linking Gadget Remote");

//          using std::placeholders::_1;
//          using std::placeholders::_2;
//          using std::placeholders::_3;
//
//          buf_gadget->setGadgetRemoteCallback(std::bind(&updateCharacteristicOnBridge, _1, _2, _3));
//          buf_gadget->setEventRemoteCallback(std::bind(&updateEventOnBridge, _1, _2));
        }

        // Code Remote
        if (remote_bf[1]) {
          logger_i("System", "Linking Code Remote");

          for (int method_index = 0; method_index < GadgetMethodCount; method_index++) {
            std::stringstream ss;
            ss << method_index;
            std::string method_str = ss.str();
            if (code_config.containsKey(method_str)) {
              JsonArray code_arr = code_config[method_str].as<JsonArray>();
              for (int i = 0; i < code_arr.size(); i++) {
                unsigned long code = code_arr[i].as<unsigned long>();
                buf_gadget->setMethodForCode((GadgetMethod) method_index, code);
              }
            }
          }

          buf_gadget->printMapping();
        }

        // Event Remote
        if (remote_bf[2]) {
          logger_i("System", "Linking Gadget Remote");
          // TODO: init event remote on gadgets
          logger_e("System", "Not Implemented");
        }

        // IR Gadget
        bool ir_ok = true;
        if (GadgetFactory::gadgetRequiresIR(gadget_ident)) {
          if (ir_gadget != nullptr) {
            logger_i("System", "Linking IR gadget");
            buf_gadget->setIR(ir_gadget);
          } else {
            logger_e("System", "No IR gadget available");
            ir_ok = false;
          }
        } else {
          logger_i("System", "No IR required");
        }

        // Radio
        // TODO: check when radio is implemented
        bool radio_ok = true;
        if (GadgetFactory::gadgetRequiresRadio(gadget_ident)) {
          if (radio_gadget != nullptr) {
            logger_i("System", "Linking radio gadget");
            buf_gadget->setRadio(radio_gadget);
          } else {
            logger_i("System", "No radio gadget available");
            radio_ok = false;
          }
        } else {
          logger_i("System", "No radio required");
        }

        // Add created gadget to the list
        if (ir_ok && radio_ok) {
//          gadgets.addGadget(buf_gadget);
        } else {
          logger_i("System", "Gadget initialization failed due to ir/radio problems");
        }
      } else {
        logger_e("System", "Gadget could not be created");
      }
      #endif
    } else {
      logger_e("System", "Error in config deserialization process");
    }
  }

  return true;
}

bool ClientMain::initEventMapping(const EventConfig &config) {
  return true;
}

bool ClientMain::initApi() {
  api_manager_ = std::make_shared<ApiManager>(this, network_);
  return true;
}

void ClientMain::handleGadgetUpdate(GadgetMeta gadget) {

}

void ClientMain::handleCode(CodeCommand code) {

}

void ClientMain::handleEvent(Event event) {

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
