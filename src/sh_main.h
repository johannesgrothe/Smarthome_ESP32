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
  gadget_tuple err_gadget(0, 0, {0, 0, 0, 0, 0}, "", "", "");
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
static bool writeGadget(uint8_t gadget_type, uint8_t remote_bf, pin_set ports, const std::string& name, const std::string& gadget_config, const std::string& code_config) {
  if (gadget_type) {
    for (int i = 0; i < GADGET_PIN_BLOCK_LEN; i++) {
      auto buf_pin = getPinForPort(ports[i]);
      if (!buf_pin) {
        return false;
      }
      ports[i] = buf_pin;
    }

    return System_Storage::writeGadget(gadget_type, remote_bf, ports, name, gadget_config, code_config);
  } else {
    return false;
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