// Connectors
#include "connectors/ir_connector.h"
#include "connectors/mqtt_connector.h"
#include "connectors/rest_connector.h"

// Gadget
#include "gadgets/sh_gadget.h"

// External Dependencies
#include "Client.h"
#include <WiFi.h>
#include "ArduinoJson.h"

#include "wifi_credentials.h"


const char json_str[] = R"(
{
  "clients": [
    {
      "type": "sh_lamp_neopixel_basic"
    },
    {
      "type": "sh_lamp_neopixel"
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
    "rest": {}
  }
}
)";

class SH_Main {
private:
  IR_Gadget * ir_gadget;
  MQTT_Gadget * mqtt_gadget;
  REST_Gadget * rest_gadget;

  WiFiClient network_client;

  SH_Gadget gadgets[];

  bool init_gadgets(JsonArray gadget_json) {
    Serial.printf("[SETUP] Initializing Gadgets: %d\n", gadget_json.size());
    add_gadget(gadget_json[0]);

    return false;
  }

  bool add_gadget(JsonObject gadget_json) {
    Serial.printf("   => Gadget Keys: %d\n", gadget_json.size());

    return false;
  }

  bool init_connectors(JsonObject connectors_json) {
    Serial.printf("[SETUP] Initializing Connectors: %d\n", connectors_json.size());
    if (connectors_json["ir"] != nullptr) {
      ir_gadget = new IR_Gadget(connectors_json["ir"].as<JsonObject>());
    }

    if (connectors_json["mqtt"] != nullptr) {
      mqtt_gadget = new MQTT_Gadget(connectors_json["mqtt"].as<JsonObject>(), &network_client);
    }

    if (connectors_json["rest"] != nullptr) {
//      rest_gadget = new REST_Gadget(connectors_json["rest"].as<JsonObject>());
    }
    return false;
  }

  bool init_network(JsonObject json) {
    if (strcmp(json["type"].as<char *>(), "wifi") == 0) {
      Serial.println("[SETUP] Creating Network: WiFi");
      network_client = WiFiClient();

      const char * ssid = json["config"]["ssid"].as<char *>();
      const char * passwd = json["config"]["password"].as<char *>();
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
        delay(2500);
        Serial.print(".");
        connection_tries ++;
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

    return everything_ok;
  }

  void test_stuff() {
//    ir_gadget->sendIR(12312312312, 1);
  }


public:
  void init() {

    DynamicJsonDocument json_file(2048);
    deserializeJson(json_file, json_str);
    JsonObject json = json_file.as<JsonObject>();

    init_network(json["network"]);
    init_connectors(json["connectors"]);
    init_gadgets(json["gadgets"].as<JsonArray>());

    test_initialization();
    ir_gadget->init();
  }

  void refresh() {
    ir_gadget->refresh();
    test_stuff();
  }
};