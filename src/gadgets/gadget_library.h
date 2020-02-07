//Normal Imports
#include "ArduinoJson.h"

// Gadget
#include "sh_gadget.h"

//Lamps
#include "sh_lamp_neopixel.h"
#include "sh_lamp_neopixel_basic.h"
#include "sh_lamp_basic.h"




static SH_Gadget * create_gadget(JsonObject gadget_json) {
  const char *name = gadget_json["name"].as<const char *>();
  const char *type = gadget_json["type"].as<const char *>();

  SH_Gadget *new_gadget;

  if (name != nullptr && type != nullptr) {
    Serial.printf("   [INFO] Adding Gadget '%s'\n", name);

    if (strcmp(type, "sh_lamp_neopixel_basic") == 0) {
      new_gadget = new SH_Lamp_NeoPixel_Basic(gadget_json);

    } else if (strcmp(type, "sh_lamp_basic") == 0) {
      new_gadget = new SH_Lamp_Basic(gadget_json);

    } else {
      Serial.printf("     => [ERR] Cannot find type '%s'\n", type);
      return nullptr;
    }
    return new_gadget;
  }
  Serial.printf("     => [ERR] Information incomplete.\n");
  return nullptr;
}