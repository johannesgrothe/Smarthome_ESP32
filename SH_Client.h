#ifndef __SH_Client__
#define __SH_Client__

#include "IPAddress.h"
#include "Client.h"
#include "ArduinoJson.h"
#include <WiFi.h>
#include "wifi_credentials.h"
#include "SH_Wallswitch.h"
#include "SH_LED.h"
#include "SH_Debug_LED.h"

#include <Arduino.h>
#include <PubSubClient.h>

#define MQTT_SH_TOPIC "homebridge/to"

#define MQTT_SH_ID "/testESP"

#define MAX_GADGETS 20

class SH_Client
{
private:
  long lastMsg;
  char msg[50];
  int value;

  IPAddress mqttServer;
  WiFiClient networkClient;
  PubSubClient mqttClient;

  uint8_t gadgets_pointer;
  SH_Gadget * gadgets[MAX_GADGETS];

  bool initialized;

  void reconnect();
  void setup_wifi();
  void setup_mqtt();

  void callback(char* topic, byte* payload, unsigned int length)
  {
    char message[length+1];
    for (uint32_t i=0; i<length; i++)
    {
      message[i] = (char) payload[i];
    }
    message[length] = '\0';

    if ((strcmp(topic, "debug/in") == 0))
    {
      Serial.printf("[DEBUG]: %s\n", message);
    }
    else if ((strcmp(topic, "homebridge/from/set") == 0))
    {
      Serial.printf("[HOMEBRIDGE SET]: %s\n", message);
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, message);
      bool fw = forwardCommand(&doc);
    }
    else if ((strcmp(topic, "homebridge/from/response") == 0))
    {
      Serial.printf("[RESPONSE]: %s\n", message);
    }
    else
    {
      Serial.printf("[MSG '%s']: %s\n", topic, message);
    }
  }

public:
  SH_Client(IPAddress server, uint8_t board_type):
    mqttServer(server),
    mqttClient(networkClient),
    gadgets(),
    lastMsg(0),
    value(0),
    initialized(false)
    {
    };

  bool init();

  void refresh();

  bool addGadget(SH_Gadget * gadget);

  bool unregisterGadget(SH_Gadget * gadget);

  bool registerGadget(SH_Gadget * gadget);

  bool forwardCommand(DynamicJsonDocument * doc);
};

#endif
