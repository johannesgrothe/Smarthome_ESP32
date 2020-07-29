#pragma once

// Settings
#include "user_settings.h"
#include "system_settings.h"

// Network Gadgets
#include "connectors/ir_gadget.h"
#include "connectors/serial_gadget.h"
#include "connectors/radio_gadget.h"

// Gadget-Lib
#include "gadgets/gadget_library.h"

// Tools
#include "console_logger.h"
#include "system_timer.h"

// External Dependencies
#include "Client.h"
#include "ArduinoJson.h"

//#include "remotes/homebridge_remote.h"
#include "remotes/smarthome_gadget_remote.h"
#include "gadget_collection.h"
#include "system_storage.h"

#include "remotes/smarthome_code_remote.h"


#include "color.h"

static void rebootChip(const char *reason) {
  if (reason != nullptr) {
    logger.print("Rebooting Chip because: '");
    logger.add(reason);
    logger.add("' in ");
  } else {
    logger.println("Rebooting Chip in ");
  }
  for (byte k = 0; k < 5; k++) {
    logger.add(5 - k);
    logger.add(" ");
    delay(1000);
  }
  ESP.restart();
}

class SH_Main {
private:

  char client_name[CLIENT_NAME_LEN_MAX]{};

  IR_Gadget *ir_gadget;
  Radio_Gadget *radio_gadget;

  Request_Gadget *network_gadget;

  Gadget_Collection gadgets;

  CodeRemote *code_remote;

  GadgetRemote *gadget_remote;

  bool initGadgets(JsonArray gadget_json);

  void mapConnectors(JsonObject connectors_json);

  bool initConnectors(JsonObject connectors_json);

  bool initNetwork(JsonObject json);

  void handleCodeConnector(Code_Gadget *gadget);

  void handleRequestConnector(Request_Gadget *gadget);

  void handleSystemRequest(Request *req);

  void handleRequest(Request *req);

  void updateGadgetRemote(const char *gadget_name, const char *service, const char *characteristic, int value);

  bool initGadgetRemote(JsonObject json);

  bool initCodeRemote(JsonObject json);

  void testStuff();

public:

  void init();

  void refresh();

  void refreshNetwork();
};