#pragma once

#include <string>
#include "../system/gadget_definitions.h"

class GadgetMeta {
public:
    const gadget_definitions::GadgetIdentifier type;
    const std::string name;

    GadgetMeta(gadget_definitions::GadgetIdentifier type, std::string name);
};
