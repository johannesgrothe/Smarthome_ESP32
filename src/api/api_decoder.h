#pragma once

#include "gadget_meta.h"
#include "../storage/system_config.h"
#include "../storage/eol_config.h"
#include "../storage/gadget_config.h"
#include "gadget_update_meta.h"

#include <ArduinoJson.h>

class ApiDecoder {
public:
    static GadgetMeta decodeGadget(DynamicJsonDocument gadget_data);

    static GadgetUpdateMeta decodeGadgetUpdateInfo(DynamicJsonDocument gadget_data);

    static std::shared_ptr<SystemConfig> decodeSystemConfig(const DynamicJsonDocument &data);

    static std::shared_ptr<EolConfig> decodeEolConfig(const DynamicJsonDocument &data);
};
