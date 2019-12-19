#include "SH_Client.h"
#include "colors.h"

#define BUILTIN_LED 2

IPAddress mqtt_broker_ip(192, 168, 178, 60);

// CLient
SH_Client shclient(mqtt_broker_ip, 0);

// Gadgets
SH_Debug_LED led1("TestLED");
SH_Debug_LED led2("TestLED2");
SH_Debug_LED led3("FriLED");

void setup()
{
  pinMode(BUILTIN_LED, OUTPUT);
  Serial.begin(115200);

  shclient.init();

  int rgb[] = {22, 255, 44};

  float hsl[3];

  rgbToHsl(rgb[0], rgb[1], rgb[2], &hsl[0]);

  Serial.printf("RGB: (%d, %d, %d), HSL: (%lf, %lf, %lf)\n", rgb[0], rgb[1], rgb[2], hsl[0] * 360, hsl[1] * 100, hsl[2] * 100);
  
  shclient.addGadget(&led1);
  shclient.addGadget(&led2);
  shclient.addGadget(&led3);
}

void loop()
{
  shclient.refresh();
}
