#pragma once

#include <string>
#include <ArduinoJson.h>

class GadgetUpdateDTO {
public:
    const std::string id;
    DynamicJsonDocument properties;

    GadgetUpdateDTO(std::string id, DynamicJsonDocument properties);
};
