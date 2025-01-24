#pragma once

#include <string>
#include <ArduinoJson.h>

#include "../system/gadget_definitions.h"

class GadgetMeta {
public:
    const gadget_definitions::GadgetIdentifier type;
    const std::string name;
    DynamicJsonDocument properties;

    GadgetMeta(gadget_definitions::GadgetIdentifier type, std::string name, DynamicJsonDocument properties);
};
