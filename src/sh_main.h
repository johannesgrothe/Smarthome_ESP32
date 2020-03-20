// Settings
#include "user_settings.h"
#include "system_settings.h"

// Connectors
#include "connectors/ir_gadget.h"
#include "connectors/rest_gadget.h"
#include "connectors/serial_gadget.h"
#include "connectors/radio_gadget.h"

// Gadget-Lib
#include "gadgets/gadget_library.h"

// Tools
#include "console_logger.h"

// External Dependencies
#include "Client.h"
#include <WiFi.h>
#include "ArduinoJson.h"

#include "wifi_credentials.h"
#include "remotes/homebridge_remote.h"
#include "gadget_collection.h"
//#include "system_storage.h"

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

//  System_Storage *storage;

  IR_Gadget *ir_gadget;
  MQTT_Gadget *mqtt_gadget;
  REST_Gadget *rest_gadget;
  Serial_Gadget *serial_gadget;
  Radio_Gadget *radio_gadget;

  WiFiClient network_client;

  Gadget_Collection gadgets;

  Remote *remotes[REMOTE_MANAGER_MAX_REMOTES]{};

  byte remote_count;

  bool initGadgets(JsonArray gadget_json) {
    gadgets = Gadget_Collection();
    byte new_gadget_count = gadget_json.size() < MAIN_MAX_GADGETS ? gadget_json.size() : MAIN_MAX_GADGETS;
    logger.print(LOG_INFO, "Creating Gadgets: ");
    logger.addln(new_gadget_count);
    logger.incIndent();
    bool everything_ok = true;
    for (unsigned int pointer = 0; pointer < new_gadget_count; pointer++) {
      JsonObject gadget = gadget_json[pointer].as<JsonObject>();
      SH_Gadget *buffergadget = createGadget(gadget);
      using std::placeholders::_1;
      using std::placeholders::_2;
      using std::placeholders::_3;
      using std::placeholders::_4;
      buffergadget->initRemoteUpdate(std::bind(&SH_Main::updateRemotes, this, _1, _2, _3, _4));
      gadgets.addGadget(buffergadget);
      delay(500);
    }
    logger.decIndent();
    return everything_ok;
  }

  void mapConnectors(JsonObject connectors_json) {
    // Mapping Code Connectors (IR/Radio) to the Gadgets for them to use
    logger.println("Mapping Connectors:");
    logger.incIndent();
    // IR
    logger.print("IR:");
    if (connectors_json["ir"] != nullptr && connectors_json["ir"].as<JsonArray>().size() > 0) {
      logger.addln();
      logger.incIndent();
      JsonArray map_gadgets = connectors_json["ir"].as<JsonArray>();
      for (auto &&map_gadget : map_gadgets) {
        const char *gadget_name = map_gadget.as<const char *>();
        SH_Gadget *found_gadget = gadgets.getGadget(gadget_name);
        if (found_gadget != nullptr) {
          found_gadget->setIR(ir_gadget);
          logger.println(LOG_DATA, gadget_name);
        }
      }
      logger.decIndent();
    } else {
      logger.addln(" -");
    }

    // Radio
    logger.print("Radio:");
    if (connectors_json["radio"] != nullptr && connectors_json["radio"].as<JsonArray>().size() > 0) {
      logger.addln();
      logger.incIndent();
      JsonArray map_gadgets = connectors_json["radio"].as<JsonArray>();
      for (auto &&map_gadget : map_gadgets) {
        const char *gadget_name = map_gadget.as<const char *>();
        SH_Gadget *found_gadget = gadgets.getGadget(gadget_name);
        if (found_gadget != nullptr) {
          found_gadget->setRadio(radio_gadget);
          logger.println(LOG_DATA, gadget_name);
        }
      }
      logger.decIndent();
    } else {
      logger.addln(" -");
    }
    logger.decIndent();
  }

  bool initConnectors(JsonObject connectors_json) {
    logger.print("Initializing Connectors: ");
    logger.addln(connectors_json.size());
    logger.incIndent();
    if (connectors_json["ir"] != nullptr) {
      ir_gadget = new IR_Gadget(connectors_json["ir"].as<JsonObject>());
    } else {
      ir_gadget = new IR_Gadget();
    }

    if (connectors_json["mqtt"] != nullptr) {
      mqtt_gadget = new MQTT_Gadget(connectors_json["mqtt"].as<JsonObject>(), &network_client);
    } else {
      mqtt_gadget = new MQTT_Gadget();
    }

    if (connectors_json["rest"] != nullptr) {
      rest_gadget = new REST_Gadget(connectors_json["rest"].as<JsonObject>(), &network_client);
    } else {
      rest_gadget = new REST_Gadget();
    }

    if (connectors_json["serial"] != nullptr) {
      serial_gadget = new Serial_Gadget(connectors_json["serial"].as<JsonObject>());
    } else {
      serial_gadget = new Serial_Gadget();
    }
    logger.decIndent();
    return true;
  }

  bool initNetwork(JsonObject json) {
    if (strcmp(json["type"].as<char *>(), "wifi") == 0) {
      logger.println("Creating Network: WiFi");
      logger.incIndent();
      network_client = WiFiClient();

      const char *ssid = json["config"]["ssid"].as<char *>();
      const char *passwd = json["config"]["password"].as<char *>();
      ssid = WIFI_SSID;
      passwd = WIFI_PW;

      logger.print(LOG_DATA, "");
      logger.add("Connecting to ");
      logger.add(ssid);

      byte connection_tries = 0;

      while (WiFiClass::status() != WL_CONNECTED && connection_tries < 6) {
        WiFi.begin(ssid, passwd);
        delay(1000);
        logger.add(".");
        connection_tries++;
      }
      logger.addln();
      if (WiFiClass::status() != WL_CONNECTED) {
        logger.println(LOG_DATA, "could not establish WiFi Connection...");
      } else {
        randomSeed(micros());
        logger.println(LOG_DATA, "WiFi connected");
        logger.print(LOG_DATA, "IP address: ");
        logger.addln(WiFi.localIP());
      }
    } else {
      logger.println(LOG_ERR, "Unknown Network Settings");
    }
    logger.decIndent();
    return false;
  }

  void testStuff() {
    logger.println("Testing Stuff");
    logger.incIndent();
    //    rest_gadget->sendRequest(REQ_HTTP_POST, "text/plain", IPAddress(192, 168, 178, 111), 3005, "/irgendein/scheiss",
//                             "pennerus maximus schmongus");
    logger.decIndent();
  }

  void refreshCodeConnector(Code_Gadget *gadget) {
    // Refresh Command
    gadget->refresh();

    // Check if Gadgets have new Commands
    if (gadget->hasNewCommand()) {
      unsigned long com = gadget->getCommand();
      logger.print("[HEX] Hex-Com: 0x");
      logger.addln(com, HEX);
      forwardCommand(com);
    }
  }

  void refreshRequestConnector(Request_Gadget *gadget) {
    // Refresh Command
    gadget->refresh();

    // Check if Gadgets have new Commands
    if (gadget->hasRequest()) {
      char type[REQUEST_TYPE_LEN_MAX]{};
      REQUEST_TYPE req_type = gadget->getRequestType();
      const char *req_body = gadget->getRequestBody();
      const char *req_path = gadget->getRequestPath();
      if (req_type == REQ_UNKNOWN)
        strncpy(type, "UWH0T??", REQUEST_TYPE_LEN_MAX);
      else if (req_type == REQ_HTTP_GET)
        strncpy(type, "GET", REQUEST_TYPE_LEN_MAX);
      else if (req_type == REQ_HTTP_POST)
        strncpy(type, "POST", REQUEST_TYPE_LEN_MAX);
      else if (req_type == REQ_HTTP_DELETE)
        strncpy(type, "DELETE", REQUEST_TYPE_LEN_MAX);
      else if (req_type == REQ_HTTP_PUT)
        strncpy(type, "PUT", REQUEST_TYPE_LEN_MAX);
      else if (req_type == REQ_MQTT)
        strncpy(type, "MQTT", REQUEST_TYPE_LEN_MAX);
      else if (req_type == REQ_SERIAL)
        strncpy(type, "Serial", REQUEST_TYPE_LEN_MAX);
      else
        strncpy(type, "{|_(-):(-)_|}", REQUEST_TYPE_LEN_MAX);

      gadget->sendAnswer("bullshit", 200);
      logger.print("[");
      logger.add(type);
      logger.add("] '");
      logger.add(req_path);
      logger.add("' :");
      logger.addln(req_body);
      handleRequest(req_type, req_path, req_body);
    }
//    if (gadget->hasResponse()) {
//
//      logger.print("[");
//      logger.add(gadget->getResponseStatusCode());
//      logger.add("] '");
//      logger.add(gadget->getResponsePath());
//      logger.add("' :");
//      logger.addln(gadget->getResponseBody());
//    }
  }

  void forwardCommand(unsigned long code) {
    logger.incIndent();
    for (byte c = 0; c < gadgets.getGadgetCount(); c++) {
      gadgets.getGadget(c)->handleCodeUpdate(code);
    }
    logger.decIndent();
  }

  void handleStringRequest(REQUEST_TYPE type, const char *path, const char *body) {
    logger.println("Forwarding String-Request to Remotes:");
    logger.incIndent();
    forwardRequest(type, path, body);
    logger.decIndent();
  }

  void handleJsonRequest(REQUEST_TYPE type, const char *path, JsonObject body) {
    logger.print("Forwarding Json-Request to ");
    logger.add(remote_count);
    logger.addln(" Remotes:");
    logger.incIndent();
    forwardRequest(type, path, body);
    logger.decIndent();
  }

  void handleRequest(REQUEST_TYPE type, const char *path, const char *body) {
    logger.incIndent();
    if (body != nullptr) {
      char first_char = body[0];
      unsigned int last_pos = strlen(body) - 1;
      if (last_pos > 0) {
        char last_char = body[last_pos];
        if (first_char == '{' && last_char == '}') {
          try {
            DynamicJsonDocument json_file(2048);
            deserializeJson(json_file, body);
            JsonObject json_doc = json_file.as<JsonObject>();
            handleJsonRequest(type, path, json_doc);
          }
          catch (DeserializationError &e) {
            handleStringRequest(type, path, body);
          }
        } else {
          handleStringRequest(type, path, body);
        }
      }
    }
    logger.decIndent();
  }

  void refreshConnectors() {
    refreshCodeConnector(serial_gadget);
    refreshCodeConnector(ir_gadget);
//    refreshCodeConnector(radio_gadget);
    refreshRequestConnector(rest_gadget);
    refreshRequestConnector(mqtt_gadget);
    refreshRequestConnector(serial_gadget);
  }

  bool initRemotes(JsonObject json) {
    logger.println("Initializing Remotes");
    logger.incIndent();

    if (json["homebridge"] != nullptr) {
      JsonArray gadget_list = json["homebridge"].as<JsonArray>();
      if (gadget_list.size() > 0) {
        logger.println(LOG_DATA, "Homebridge");
        logger.incIndent();
        auto *homebridge_remote = new Homebridge_Remote(mqtt_gadget);
        for (auto &&gadget_name_str : gadget_list) {
          const char *gadget_name = gadget_name_str.as<const char *>();
          SH_Gadget *gadget = gadgets.getGadget(gadget_name);
          homebridge_remote->addGadget(gadget);
        }
        logger.decIndent();
        addRemote(homebridge_remote);
      }
    }
    logger.decIndent();
    return true;
  }

  void updateRemotes(const char *gadget_name, const char *service, const char *characteristic, int value) {
    for (byte k = 0; k < remote_count; k++) {
      remotes[k]->updateCharacteristic(gadget_name, service, characteristic, value);
    }
  }

  void forwardRequest(REQUEST_TYPE type, const char *path, const char *body) {
    for (byte k = 0; k < remote_count; k++) {
      remotes[k]->handleRequest(path, type, body);
    }
  }

  void forwardRequest(REQUEST_TYPE type, const char *path, JsonObject body) {
    for (byte k = 0; k < remote_count; k++) {
      remotes[k]->handleRequest(path, type, body);
    }
  }

  void addRemote(Remote *new_remote) {
    if (remote_count < (REMOTE_MANAGER_MAX_REMOTES - 1)) {
      remotes[remote_count] = new_remote;
      remote_count++;
    }
  }

public:

  void init() {

    Serial.begin(SERIAL_SPEED);

    logger.println(LOG_INFO, "Launching...");

//    storage = System_Storage();

    logger.println(LOG_INFO, "Loading Config...");

    logger.incIndent();
    DynamicJsonDocument json_file(2048);
    try {
      deserializeJson(json_file, json_str);
    }
    catch (DeserializationError &e) {
      logger.println(LOG_ERR, "Cannot read JSON, creating blank Config.");
      deserializeJson(json_file, default_config);
    }
    JsonObject json = json_file.as<JsonObject>();

    logger.decIndent();

    initNetwork(json["network"]);
    initConnectors(json["connectors"]);
    initGadgets(json["gadgets"]);
    mapConnectors(json["connector-mapping"]);
    initRemotes(json["remote-mapping"]);

    testStuff();
    logger.print("Free Heap: ");
    logger.add(ESP.getFreeHeap());
    logger.addln();
  }

  void refresh() {
    refreshConnectors();

    for (byte c = 0; c < gadgets.getGadgetCount(); c++) {
      gadgets.getGadget(c)->refresh();
    }
  }

};
