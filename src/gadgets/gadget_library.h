#pragma once

//Normal Imports
#include "ArduinoJson.h"
#include <memory>
#include "../console_logger.h"
#include "system_storage.h"

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

static std::shared_ptr<SH_Gadget> createGadget(GadgetIdentifier gadget_type, pin_set pins, const std::string& name, JsonObject gadget_config, JsonObject code_config) {
  switch (gadget_type) {
    case GadgetIdentifier::sh_doorbell_basic:
      return createSHDoorbellBasic(name, pins, gadget_config);

    case GadgetIdentifier::sh_lamp_neopixel_basic:
      return std::make_shared<SH_Lamp_NeoPixel_Basic>(gadget_config);

    case GadgetIdentifier::sh_lamp_basic:
      return std::make_shared<SH_Lamp_Basic>(gadget_config);

    case GadgetIdentifier::sh_fan_westinghouse_ir:
      return std::make_shared<SH_Fan_Westinghouse_IR>(gadget_config);

    case GadgetIdentifier::sh_lamp_westinghouse_ir:
      return std::make_shared<SH_Lamp_Westinghouse_IR>(gadget_config);

    default:
      return nullptr;
  }
}
