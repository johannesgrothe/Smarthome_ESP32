#pragma once

#include "gadget_meta.h"
#include "client_meta.h"
#include "../console_logger.h"
#include "../storage/system_config.h"
#include "../storage/event_config.h"
#include "../storage/gadget_config.h"
#include "gadget_update_meta.h"

#include <ArduinoJson.h>

class ApiDecoder {
public:

  static GadgetMeta decodeGadget(DynamicJsonDocument gadget_data);

  static GadgetUpdateMeta decodeGadgetUpdateInfo(DynamicJsonDocument gadget_data);

  static CharacteristicMeta decodeCharacteristic(DynamicJsonDocument characteristic_data);

  static std::shared_ptr<SystemConfig> decodeSystemConfig(const DynamicJsonDocument& data);

  static std::shared_ptr<EventConfig> decodeEventConfig(const DynamicJsonDocument& data);

  static std::shared_ptr<GadgetConfig> decodeGadgetConfig(const DynamicJsonDocument& data);

};
