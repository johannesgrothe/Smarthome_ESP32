#pragma once

#include <string>
#include <ArduinoJson.h>

class GadgetUpdateMeta {
public:
    const std::string id;
    DynamicJsonDocument properties;

    GadgetUpdateMeta(std::string id, DynamicJsonDocument properties);
};
