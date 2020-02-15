// Settings
#include "user_settings.h"
#include "system_settings.h"

// Connectors
#include "connectors/ir_connector.h"
#include "connectors/mqtt_connector.h"
#include "connectors/rest_connector.h"
#include "connectors/serial_connector.h"
#include "connectors/radio_connector.h"

// Gadget-Lib
#include "gadgets/gadget_library.h"

// Tools
#include "console_logger.h"

// External Dependencies
#include "Client.h"
#include <WiFi.h>
#include "ArduinoJson.h"
#include <EEPROM.h>

#include "wifi_credentials.h"

//test-commit

class SH_Main {
private:
  IR_Gadget *ir_gadget;
  MQTT_Gadget *mqtt_gadget;
  REST_Gadget *rest_gadget;
  Serial_Gadget *serial_gadget;
  Radio_Connector *radio_gadget;

  WiFiClient network_client;

  SH_Gadget *gadgets[MAIN_MAX_GADGETS];

  byte anz_gadgets = 0;

  bool init_gadgets(JsonArray gadget_json) {
    anz_gadgets = gadget_json.size() < MAIN_MAX_GADGETS ? gadget_json.size() : MAIN_MAX_GADGETS;
    logger.print(LOG_INFO, "Creating Gadgets: ");
    logger.addln(anz_gadgets);
    logger.incIntent();
    bool everything_ok = true;
    for (unsigned int pointer = 0; pointer < anz_gadgets; pointer++) {
      JsonObject gadget = gadget_json[pointer].as<JsonObject>();
      gadgets[pointer] = create_gadget(gadget);
      everything_ok = everything_ok && gadgets[pointer]->init();
    }
    logger.decIntent();
    return everything_ok;
  }

  bool init_connectors(JsonObject connectors_json) {
    logger.print("Initializing Connectors: ");
    logger.addln(connectors_json.size());
    logger.incIntent();
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

//    if (connectors_json["rest"] != nullptr) {
////      rest_gadget = new REST_Gadget(connectors_json["rest"].as<JsonObject>());
//    } else {
////      rest_gadget = new REST_Gadget();
//    }

    if (connectors_json["serial"] != nullptr) {
      serial_gadget = new Serial_Gadget(connectors_json["serial"].as<JsonObject>());
    } else {
      serial_gadget = new Serial_Gadget();
    }
    logger.decIntent();
    return true;
  }

  bool init_network(JsonObject json) {
    if (strcmp(json["type"].as<char *>(), "wifi") == 0) {
      logger.println("Creating Network: WiFi");
      logger.incIntent();
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
    logger.decIntent();
    return false;
  }

  bool test_initialization() {
    logger.println("Testing initialization:");
    logger.incIntent();

    bool everything_ok = true;

    logger.print(LOG_DATA, "IR: ");
    if (ir_gadget->isInitialized()) {
      logger.addln("OK");
    } else {
      logger.addln("ERR");
      everything_ok = false;
    }

    logger.print(LOG_DATA, "MQTT: ");
    if (mqtt_gadget->isInitialized()) {
      logger.addln("OK");
    } else {
      logger.addln("ERR");
      everything_ok = false;
    }

    logger.print(LOG_DATA, "Serial: ");
    if (serial_gadget->isInitialized()) {
      logger.addln("OK");
    } else {
      logger.addln("ERR");
      everything_ok = false;
    }
    logger.decIntent();
    return everything_ok;
  }

  void test_stuff() {
    logger.println("Testing Stuff");
    logger.incIntent();
    for (int c = 0; c < anz_gadgets; c++) {
//      gadgets[c]->printMapping();
    }
    logger.decIntent();
  }

  void rebootChip() {
    rebootChip(nullptr);
  }

  void rebootChip(const char * reason) {
    if (reason != nullptr) {
      logger.print("Rebooting Chip because: '");
      logger.add(reason);
      logger.add("' in ");
    } else {
      logger.println("Rebooting Chip in ");
    }
    for (byte k = 0; k < 5; k++) {
      logger.add(5-k);
      logger.add(" ");
      delay(1000);
    }
    ESP.restart();
  }

  void decodeStringCommand(const char * message, unsigned int length) {
    std::string com = message;

    if (com.rfind("_sys:", 0) == 0) {
      logger.print("System Command Detected: ");
      if (com.rfind("_sys:flash", 0) == 0) {
        logger.addln("flash");
        char input_json[900]{};
      } else if (com.rfind("_sys:reboot", 0) == 0) {
        logger.addln("reboot");
        rebootChip("Input Command");
      } else {
        logger.addln("<unknown>");
      }
    } else if (com.rfind("_dev:", 0) == 0) {
      logger.print("Development Command Detected: ");
      if (com.rfind("_dev:log_on", 4) == 0) {
        logger.addln("log_on");
        logger.activateLogging();
      } else if (com.rfind("_dev:log_off", 4) == 0) {
        logger.addln("log_off");
        logger.deactivateLogging();
      } else {
        logger.addln("<unknown>");
      }
    }
  }

  void refreshConnector(Code_Gadget *gadget) {
    // Refresh Command
    gadget->refresh();

    // Check if Gadgets have new Commands
    if (gadget->hasNewCommand()) {
      if (gadget->hasHexCommand()) {
        unsigned long com = gadget->getCommandHEX();
        logger.print("[Serial] Hex-Com: 0x");
        Serial.println(com, HEX);
        forwardCommand(com);
      } else {
        unsigned int length = gadget->getCommandLength();

        logger.print("[Serial] String-Com: (");
        logger.add(length);
        logger.add(") '");
        logger.add(gadget->getCommandStr());
        logger.addln("'");

        decodeStringCommand(gadget->getCommandStr(), length);
      }
    }
  }


//  EEPROM.write(0, code);
//  EEPROM.commit();

public:
  void init() {

    Serial.begin(115200);

    EEPROM.begin(1023);

    Serial.println(EEPROM.read(0));

    logger.println(LOG_INFO, "Launching...");
    logger.println(LOG_INFO, "Loading Config...");
    logger.incIntent();
    DynamicJsonDocument json_file(2048);
    try {
      deserializeJson(json_file, json_str);
    }
    catch(std::exception& e ) {
      logger.println(LOG_ERR,"Cannot read JSON, creating blank Config.");
      deserializeJson(json_file, default_config);
    }
    JsonObject json = json_file.as<JsonObject>();

    logger.decIntent();

    init_network(json["network"]);
    init_connectors(json["connectors"]);
    init_gadgets(json["gadgets"]);

    test_initialization();

    test_stuff();
  }

  void forwardCommand(unsigned long code) {
    logger.print("Forwarding Command: 0x");
    Serial.println(code, HEX);
    logger.incIntent();
    for (byte c = 0; c < anz_gadgets; c++) {
      gadgets[c]->decodeCommand(code);
    }
    logger.decIntent();
  }

  void refreshConnectors() {
    refreshConnector(serial_gadget);
    refreshConnector(ir_gadget);
//    refreshConnector(radio_gadget);
  }

  void refresh() {
    refreshConnectors();

    for (byte c = 0; c < anz_gadgets; c++) {
      gadgets[c]->refresh();
    }
  }
};