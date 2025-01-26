#pragma once

#include "../../storage/system_config.h"
#include "../../storage/eol_config.h"
#include "../dtos/gadget_update_dto.h"
#include "../dtos/gadget_dto.h"

#include <ArduinoJson.h>

class ApiDecoder {
public:
    static GadgetDTO decodeGadget(DynamicJsonDocument gadget_data);

    static GadgetUpdateDTO decodeGadgetUpdateInfo(DynamicJsonDocument gadget_data);

    static std::shared_ptr<SystemConfig> decodeSystemConfig(const DynamicJsonDocument &data);

    static std::shared_ptr<EolConfig> decodeEolConfig(const DynamicJsonDocument &data);
};
