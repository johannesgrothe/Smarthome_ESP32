#pragma once
#include <ArduinoJson.h>

class GadgetAPIConnector {
public:
    virtual ~GadgetAPIConnector() = default;

    virtual void applyChanges(DynamicJsonDocument data) = 0;

    virtual DynamicJsonDocument encode() = 0;
};
