#include "SH_Client.h"

#define BUILTIN_LED 2

IPAddress mqtt_broker_ip(192, 168, 178, 60);

SH_Client shclient(mqtt_broker_ip, 0);

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  shclient.init();
  
  SH_Debug_LED led1("TestLED");
  shclient.addGadget(&led1);

  SH_Debug_LED led2("TestLED2");
  shclient.addGadget(&led2);
}

void loop()
{
  shclient.refresh();
}
