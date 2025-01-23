#pragma once

#include <string>
#include <ArduinoJson.h>
#include <gadgets/abstract/gadget.h>

#include "../system/gadget_definitions.h"

class GadgetMeta {
public:
    const GadgetType supertpe;
    const gadget_definitions::GadgetIdentifier type;
    const std::string name;
    DynamicJsonDocument properties;

    GadgetMeta(gadget_definitions::GadgetIdentifier type, std::string name, DynamicJsonDocument properties);
};
