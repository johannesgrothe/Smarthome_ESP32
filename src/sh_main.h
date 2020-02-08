// Connectors
#include "connectors/ir_connector.h"
#include "connectors/mqtt_connector.h"
#include "connectors/rest_connector.h"
#include "connectors/serial_connector.h"
#include "connectors/radio_connector.h"

// Gadget-Lib
#include "gadgets/gadget_library.h"

// External Dependencies
#include "Client.h"
#include <WiFi.h>
#include "ArduinoJson.h"

#include "wifi_credentials.h"


const char json_str[] = R"(
{
  "gadgets": [
    {
      "type": "sh_lamp_neopixel_basic",
      "name": "Testlampe NP",
      "lamp_type": 0,
      "pin": 23,
      "length": "1",
      "mapping": {
        "toggleStatus": [1],
        "tunOn": [2],
        "tunOff": [3]
      }
    },
    {
      "type": "sh_lamp_basic",
      "name": "Testlampe 2",
      "lamp_type": 0,
      "pin": 2,
      "default_state": 1,
      "mapping": {
        "toggleStatus": [1],
        "tunOn": [2],
        "tunOff": [3]
      }
    }
  ],
  "network": {
    "type": "wifi",
    "config": {
      "ssid": "testwlan",
      "password": "testpw"
    }
  },
  "connectors": {
    "ir": {
      "recv_pin": 11,
      "send_pin": 12
    },
    "mqtt": {
      "ip": "192.168.178.60",
      "port": "1883"
    },
    "rest": {},
    "serial": {}
  }
}
)";

class SH_Main {
private:
  IR_Gadget *ir_gadget;
  MQTT_Gadget *mqtt_gadget;
  REST_Gadget *rest_gadget;
  Serial_Gadget *serial_gadget;
  Radio_Connector *radio_gadget;

  WiFiClient network_client;

  SH_Gadget *gadgets[20];

  uint8_t anz_gadgets = 0;

  bool init_gadgets(JsonArray gadget_json) {
    anz_gadgets = (uint8_t) gadget_json.size();
    Serial.printf("[SETUP] Initializing Gadgets: %d\n", gadget_json.size());
    bool everything_ok = true;
    uint8_t counter = 0;
    for (unsigned int pointer = 0; pointer < gadget_json.size(); pointer++) {
//    for (auto && pointer : gadget_json) {
      JsonObject gadget = gadget_json[pointer].as<JsonObject>();
      gadgets[pointer] = create_gadget(gadget);
      everything_ok = everything_ok && gadgets[pointer]->init();
    }
    return everything_ok;
  }

  bool init_connectors(JsonObject connectors_json) {
    Serial.printf("[SETUP] Initializing Connectors: %d\n", connectors_json.size());
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

    return true;
  }

  bool init_network(JsonObject json) {
    if (strcmp(json["type"].as<char *>(), "wifi") == 0) {
      Serial.println("[SETUP] Creating Network: WiFi");
      network_client = WiFiClient();

      const char *ssid = json["config"]["ssid"].as<char *>();
      const char *passwd = json["config"]["password"].as<char *>();
      Serial.printf("   => Original Values: SSID: %s; PW: %s\n", ssid, passwd);
      ssid = WIFI_SSID;
      passwd = WIFI_PW;

      Serial.print("   => Connecting to ");
      Serial.print(ssid);

      WiFi.begin(ssid, passwd);

      uint8_t connection_tries = 0;

      while (WiFiClass::status() != WL_CONNECTED) {
        if (connection_tries > 5) {
          Serial.println("\n   => Restarting to try Again...");
          ESP.restart();
        }
        delay(1000);
        Serial.print(".");
        connection_tries++;
      }

      randomSeed(micros());

      Serial.println("");
      Serial.println("   => WiFi connected");
      Serial.print("   => IP address: ");
      Serial.println(WiFi.localIP());

    } else {
      Serial.println("[ERR] Unknown Network Settings");
    }
    return false;
  }

  bool test_initialization() {
    Serial.println("[SETUP] Testing initialization:");

    bool everything_ok = true;

    Serial.print("   => IR: ");
    if (ir_gadget->isInitialized()) {
      Serial.println("OK");
    } else {
      Serial.println("ERR");
      everything_ok = false;
    }

    Serial.print("   => MQTT: ");
    if (mqtt_gadget->isInitialized()) {
      Serial.println("OK");
    } else {
      Serial.println("ERR");
      everything_ok = false;
    }

    Serial.print("   => Serial: ");
    if (serial_gadget->isInitialized()) {
      Serial.println("OK");
    } else {
      Serial.println("ERR");
      everything_ok = false;
    }

    return everything_ok;
  }

  void test_stuff() {
    Serial.println("[INFO] Testing Stuff");
    for (int c = 0; c < anz_gadgets; c++) {
      gadgets[c]->printMapping();
    }
    forwardCommand(1);
  }


public:
  void init() {

    DynamicJsonDocument json_file(2048);
    deserializeJson(json_file, json_str);
    JsonObject json = json_file.as<JsonObject>();

    init_network(json["network"]);
    init_connectors(json["connectors"]);
    init_gadgets(json["gadgets"]);

    test_initialization();
    ir_gadget->init();

    test_stuff();
  }

  void forwardCommand(unsigned long code) {
    Serial.print("[INFO] Forwarding Command: 0x");
    Serial.println(code, HEX);
    for (uint8_t c = 0; c < anz_gadgets; c++) {
      gadgets[c]->decodeCommand(code);
    }
  }

  void refreshConnectors() {
    // Refresh All Connectors
    serial_gadget->refresh();

    // Check if Gadgets have new Commands
    if (serial_gadget->hasNewCommand()) {
      Serial.printf("[Serial] Command (%d): '%s' / 0x",
        serial_gadget->getCommandLength(),
        serial_gadget->getCommandString());
      Serial.println(serial_gadget->getCommandHEX(), HEX);
    }
    if (serial_gadget->hasHexCommand()) {
      forwardCommand(serial_gadget->getCommandHEX());
    }
  }

  void refresh() {
//    for (auto && sh_gadget : gadgets) {
//      sh_gadget->refresh();
//    }

    refreshConnectors();

    for (uint8_t c = 0; c < anz_gadgets; c++) {
      gadgets[c]->refresh();
    }
  }
};