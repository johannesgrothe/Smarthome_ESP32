// Connectors
#include "connectors/ir_connector.h"
#include "connectors/mqtt_connector.h"
#include "connectors/rest_connector.h"

// Gadget
#include "gadgets/sh_gadget.h"

// External Dependencies
#include "Client.h"
#include "ArduinoJson.h"

class SH_Main {
private:
  IR_Gadget ir_gadget;
  MQTT_Gadget mqtt_gadget;
  REST_Gadget rest_gadget;

  Client network_client;

  SH_Gadget gadgets[];

public:
  void init() {
    json_str = "{\"clients\":[],\"network\"}";

  }

  void init_gadgets(DynamicJsonDocument *doc) {
    JsonObject json = doc->as<JsonObject>();

  }

  void init_connectors(DynamicJsonDocument *doc) {
    JsonObject json = doc->as<JsonObject>();

  }


};