#include "SH_Client.h"
#include "SH_Debug_LED.h"


#define BUILTIN_LED 2

IPAddress server(192, 168, 178, 23);

SH_Client shclient(server);

SH_Debug_LED debug_led;


void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);
  shclient.init();

  debug_led.setName("LED1");
  char buffer[8];
  debug_led.getName(buffer);
  Serial.println(buffer);

  debug_led.setName("123456789");
  debug_led.getName(buffer);
  Serial.println(buffer);
  debug_led.setName("abcdefg");
  debug_led.getName(buffer);
  Serial.println(buffer);
  debug_led.setName("123456789");
  debug_led.getName(buffer);
  Serial.println(buffer);
}

void loop()
{
  shclient.refresh();
  debug_led.test();
  debug_led.setStatus(!debug_led.getStatus());
  delay(2500);
}
