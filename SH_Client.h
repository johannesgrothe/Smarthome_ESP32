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
#include <AsyncMqttClient.h>

#define MQTT_SH_TOPIC "homebridge/to"

#define MQTT_SH_ID "/testESP"

#define MAX_GADGETS 20

class SH_Client
{
private:
  WiFiClient espClient;
  long lastMsg;
  char msg[50];
  int value;
  IPAddress server;
  SH_Wallswitch testswitch;
  SH_Debug_LED testled;

  uint8_t gadgets_pointer;

  SH_Gadget gadgets[MAX_GADGETS];

  bool initialized;

  void reconnect();
  void setup_wifi();

public:
  SH_Client(IPAddress server):
  lastMsg(0),
  value(0),
  initialized(false)
  {
    testswitch.init(22, false);
    // testled.init(1, 1, 5, 200);
  };

  bool init();

  void refresh();

  bool addGadget(SH_Gadget * gadget);

  bool removeGadget(char * name);

  bool removeGadget(uint8_t index);
};

#endif
