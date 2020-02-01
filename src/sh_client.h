#ifndef __SH_Client__
#define __SH_Client__

#include "IPAddress.h"
#include "Client.h"
#include "ArduinoJson.h"
#include <WiFi.h>
#include "wifi_credentials.h"

#include "gadgets/ir_gadget.h"

#include "gadgets/sh_wallswitch.h"
#include "gadgets/sh_lamp_neopixel.h"
#include "gadgets/sh_lamp_neopixel_basic.h"
#include "gadgets/sh_lamp_basic.h"

#include <Arduino.h>
#include <PubSubClient.h>

#define MQTT_SH_TOPIC "homebridge/to"

#define MQTT_SH_ID "/testESP"

#define MAX_GADGETS 20

// #define LOG_MESSAGES

class SH_Client {
private:
  long lastMsg;
  char msg[50];
  int value;
  uint8_t boardtype;

  IPAddress mqttServer;
  WiFiClient networkClient;
  PubSubClient mqttClient;

  IR_Gadget irGadget;

  uint8_t gadgets_pointer;
  SH_Gadget *gadgets[MAX_GADGETS];

  bool initialized;

  void reconnect();

  void setup_wifi();

  void setup_mqtt();

  void callback(char *topic, byte *payload, unsigned int length) {
    char message[length + 1];
    for (uint32_t i = 0; i < length; i++) {
      message[i] = (char) payload[i];
    }
    message[length] = '\0';

    if ((strcmp(topic, "debug/in") == 0)) {
#ifdef LOG_MESSAGES
      Serial.printf("[DEBUG]: %s\n", message);
#endif
    } else if ((strcmp(topic, "homebridge/from/set") == 0)) {
#ifdef LOG_MESSAGES
      Serial.printf("[HOMEBRIDGE SET]: %s\n", message);
#endif
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, message);
      bool fw = forwardCommand(&doc);
    } else if ((strcmp(topic, "homebridge/from/response") == 0)) {
#ifdef LOG_MESSAGES
      Serial.printf("[RESPONSE]: %s\n", message);
#endif
    } else {
#ifdef LOG_MESSAGES
      Serial.printf("[MSG '%s']: %s\n", topic, message);
#endif
    }
  }

  bool publishMessage(char *topic, char *message);

public:
  SH_Client() :
    lastMsg(0),
    value(0),
    boardtype(0),
    gadgets(),
    initialized(false) {
  };

  SH_Client(IPAddress server, uint8_t board_type) :
    lastMsg(0),
    value(0),
    boardtype(board_type),
    mqttServer(server),
    mqttClient(networkClient),
    gadgets(),
    initialized(false) {
  };

  bool init();

  void refresh();

  bool addGadget(SH_Gadget *gadget);

  bool unregisterGadget(SH_Gadget *gadget);

  bool registerGadget(SH_Gadget *gadget);

  bool forwardCommand(DynamicJsonDocument *doc);
};

#endif
