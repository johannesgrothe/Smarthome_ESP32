#pragma once

// Settings
#include "user_settings.h"
#include "system_settings.h"

// Network Gadgets
#include "network_library.h"

// Gadget-Lib
#include "gadgets/gadget_library.h"

// Tools
#include "console_logger.h"
#include "system_timer.h"
#include "hardware_link.h"
#include "boot_mode.h"

// External Dependencies
#include "Client.h"
#include "ArduinoJson.h"

// Remotes
#include "remote_library.h"

#include "gadget_collection.h"
#include "system_storage.h"

#include "hardware_link.h"
#include "pin_profile.h"
#include "color.h"

static void rebootChip(const std::string& reason) {
  if (!reason.empty()) {
    logger.print("Rebooting Chip because: '");
    logger.print(reason);
    logger.print("' in ");
  } else {
    logger.println("Rebooting Chip in ");
  }
  for (byte k = 0; k < 5; k++) {
    logger.print(5 - k);
    logger.print(" ");
    delay(1000);
  }
  ESP.restart();
}

/**
 * Reads the gadget with the selected index
 * @param index index of the gadget to load
 * @return the gadget
 */
static gadget_tuple readGadget(uint8_t index) {
  auto gadget = System_Storage::readGadget(index);
  gadget_tuple err_gadget(0, {0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0}, "", "", "");
  auto type = std::get<0>(gadget);
  if (type) {
    auto ports = std::get<2>(gadget);
    for (int i = 0; i < GADGET_PIN_BLOCK_LEN; i++) {
      auto buf_pin = getPinForPort(ports[i]);
      if (!buf_pin) {
        return err_gadget;
      }
      ports[i] = buf_pin;
    }

    return gadget_tuple(type, std::get<1>(gadget), ports, std::get<3>(gadget), std::get<4>(gadget), std::get<5>(gadget));
  } else {
    return err_gadget;
  }
}

/**
 * Writes a gadget to the eeprom
 * @param gadget_type type of the gadget
 * @param remote_bf bitfield for the remotes
 * @param ports ports used by the gadget to connect hardware
 * @param gadget_config base config for the gadget
 * @param code_config config for the code mapping
 * @return whether writing was successful
 */
static status_tuple writeGadget(uint8_t gadget_type, bitfield_set remote_bf, pin_set ports, const std::string& name, const std::string& gadget_config, const std::string& code_config) {

  if (gadget_type >= GadgetIdentifierCount) {
    logger.printfln(LOG_TYPE::ERR, "Unknown gadget identifier '%d'", gadget_type);
    return status_tuple(false, "not saving gadget with err-type 0");
  }

  auto type = (GadgetIdentifier) gadget_type;

  if (type != GadgetIdentifier::None) {
    logger.printfln("Saving gadget '%s' with type %d", name.c_str(), gadget_type);

    auto existing_ports = System_Storage::readAllGadgetPorts();

    for (auto gadget_port: ports) {
      // Check if port is configured on the system
      auto buf_pin = getPinForPort(gadget_port);
      if (!buf_pin && gadget_port) {
        logger.printfln(LOG_TYPE::ERR, "Cannot save gadget: port %d is not configured on this system", gadget_port);
        return status_tuple(false, "gadget tries to use port not configured on this system");
      }

      // Check if port is already in use on the system
      for (auto existing_port: existing_ports) {
        if (gadget_port == existing_port) {
          logger.printfln(LOG_TYPE::ERR, "Cannot save gadget: gadget tries to use port already occupied (%d)", gadget_port);
          return status_tuple(false, "gadget tries to use port already occupied");
        }
      }
    }

    auto existing_names = System_Storage::readAllGadgetNames();

    for (const auto& list_name: existing_names) {
      if (name == list_name) {
        logger.printfln(LOG_TYPE::ERR, "Cannot save gadget: gadget name '%s' is already in use", name.c_str());
        return status_tuple(false, "gadget name is already in use");
      }
    }

    return System_Storage::writeGadget(gadget_type, remote_bf, ports, name, gadget_config, code_config);
  } else {
    logger.println(LOG_TYPE::ERR, "Cannot save gadget: gadget has err-type 0");
    return status_tuple(false, "not saving gadget with err-type 0");
  }
}

class SH_Main {
private:

  std::string client_id_;

  IR_Gadget *ir_gadget;
  Radio_Gadget *radio_gadget;

  Request_Gadget *network_gadget;

  Gadget_Collection gadgets;

  CodeRemote *code_remote;

  GadgetRemote *gadget_remote;

  BootMode system_mode;

  unsigned long last_req_id_;

  bool eeprom_active_;

  bool initGadgets(JsonArray gadget_json);

  void mapConnectors(JsonObject connectors_json);

  bool initConnectors();

  bool initNetwork();

  bool initNetwork(NetworkMode);

  void handleCodeConnector(Code_Gadget *gadget);

  void handleRequestConnector(Request_Gadget *gadget);

  void handleSystemRequest(Request *req);

  void handleRequest(Request *req);

  void updateGadgetRemote(const char *gadget_name, const char *service, const char *characteristic, int value);

  bool initGadgetRemote(JsonObject json);

  bool initCodeRemote(JsonObject json);

  void testStuff();

  void refreshModeSerial();

  void refreshModeNetwork();

  void refreshModeComplete();

  void initModeSerial();

  void initModeNetwork();

  void initModeComplete();

public:

  SH_Main();

  void init();

  void refresh();

  void refreshNetwork();
};