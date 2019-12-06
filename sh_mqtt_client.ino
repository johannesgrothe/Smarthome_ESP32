#include "SH_Client.h"

#define BUILTIN_LED 2

IPAddress mqtt_broker_ip(192, 168, 178, 60);

SH_Client shclient(mqtt_broker_ip, 0);

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  shclient.init();
  shclient.addGadget(SH_Debug_LED("TestLED"));
  shclient.addGadget(SH_Debug_LED("TestLED2"));
  shclient.addGadget(SH_Debug_LED("TestLED3"));
}

void loop()
{
  shclient.refresh();
}
