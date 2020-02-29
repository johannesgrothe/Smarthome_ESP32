//Normal Imports
#include "ArduinoJson.h"
#include "../console_logger.h"

// Gadget
#include "sh_gadget.h"

//Lamps
#include "sh_lamp_neopixel.h"
#include "sh_lamp_neopixel_basic.h"
#include "sh_lamp_basic.h"

//Fan
#include "sh_fan_basic.h"


static SH_Gadget *create_gadget(JsonObject gadget_json) {
  const char *name = gadget_json["name"].as<const char *>();
  const char *type = gadget_json["type"].as<const char *>();

  SH_Gadget *new_gadget;

  if (name != nullptr && type != nullptr) {
    logger.print("Adding Gadget '");
    logger.add(name);
    logger.addln("'");
    logger.incIntent();

    //sh_neopixel_basic.h
    if (strcmp(type, "sh_lamp_neopixel_basic") == 0) {
      new_gadget = new SH_Lamp_NeoPixel_Basic(gadget_json);

      // sh_lamp_basic.h
    } else if (strcmp(type, "sh_lamp_basic") == 0) {
      new_gadget = new SH_Lamp_Basic(gadget_json);

      // sh_fan_basic.h
    } else if (strcmp(type, "sh_fan_basic") == 0) {
      new_gadget = new SH_Fan_Basic(gadget_json);

    } else {
      logger.print(LOG_ERR, "Cannot find Type '");
      logger.add(type);
      logger.addln("'");
      new_gadget = nullptr;
    }
    logger.decIntent();
    return new_gadget;
  }
  logger.println(LOG_ERR, "Information incomplete.");
  return nullptr;
}