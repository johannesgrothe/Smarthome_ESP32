#include "sh_client.h"
#include "colors.h"

#define BUILTIN_LED 2

IPAddress mqtt_broker_ip(192, 168, 178, 60);

// CLient
SH_Client shclient(mqtt_broker_ip, 0);

// Gadgets
SH_Lamp_NeoPixel_Basic rgbled("NeoPixel", 23, 1);
SH_Lamp_Basic basiclamp("Basic_Lamp", BUILTIN_LED, LOW);

void setup() {
  Serial.begin(115200);

  shclient.init();

  int rgb[] = {22, 255, 44};

  float hsl[3];

  rgbToHsl(rgb[0], rgb[1], rgb[2], &hsl[0]);

  Serial.printf("RGB: (%d, %d, %d), HSL: (%.2lf, %.2lf, %.2lf)\n", rgb[0], rgb[1], rgb[2], hsl[0] * 360, hsl[1] * 100,
                hsl[2] * 100);

  shclient.addGadget(&rgbled);
  shclient.addGadget(&basiclamp);
}

void loop() {
  shclient.refresh();
}
