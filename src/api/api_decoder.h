#pragma once

#include "gadget_meta.h"
#include "client_meta.h"
#include "../console_logger.h"

#include <ArduinoJson.h>

class ApiDecoder {
public:

  static GadgetMeta decodeGadget(DynamicJsonDocument gadget_data);

  static CharacteristicMeta decodeCharacteristic(DynamicJsonDocument characteristic_data);

};
