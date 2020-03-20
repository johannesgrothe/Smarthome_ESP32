//Normal Imports
#include "ArduinoJson.h"
#include "../console_logger.h"

// Gadget
#include "sh_gadget.h"

//Lamps
#include "sh_lamp_neopixel.h"
#include "sh_lamp_neopixel_basic.h"
#include "sh_lamp_basic.h"
#include "sh_doorbell_basic.h"

//Fan
#include "sh_fan_westinghouse_ir.h"
#include "sh_lamp_westinghouse_ir.h"


static SH_Gadget *createGadget(JsonObject gadget_json) {
  const char *name = gadget_json["name"].as<const char *>();
  const char *type = gadget_json["type"].as<const char *>();

  SH_Gadget *new_gadget;

  if (name != nullptr && type != nullptr) {
    logger.print("Adding Gadget '");
    logger.add(name);
    logger.addln("'");
    logger.incIndent();

    //sh_neopixel_basic.h
    if (strcmp(type, "sh_lamp_neopixel_basic") == 0) {
      new_gadget = new SH_Lamp_NeoPixel_Basic(gadget_json);

      // sh_lamp_basic.h
    } else if (strcmp(type, "sh_lamp_basic") == 0) {
      new_gadget = new SH_Lamp_Basic(gadget_json);

      // sh_fan_westinghouse_ir.h
    } else if (strcmp(type, "sh_fan_westinghouse_ir") == 0) {
      new_gadget = new SH_Fan_Westinghouse_IR(gadget_json);

      // sh_lamp_westinghouse_ir.h
    } else if (strcmp(type, "sh_lamp_westinghouse_ir") == 0) {
      new_gadget = new SH_Lamp_Westinghouse_IR(gadget_json);

      // sh_fan_basic.h
    } else if (strcmp(type, "sh_doorbell_basic") == 0) {
      new_gadget = new SH_Doorbell_Basic(gadget_json);

    } else {
      logger.print(LOG_ERR, "Cannot find Type '");
      logger.add(type);
      logger.addln("'");
      new_gadget = nullptr;
    }
    logger.decIndent();
    return new_gadget;
  }
  logger.println(LOG_ERR, "Information incomplete.");
  return nullptr;
}