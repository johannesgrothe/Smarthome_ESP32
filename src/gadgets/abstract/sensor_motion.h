#pragma once

#include "gadget.h"

class Sensor_Motion : public Gadget {
    bool sensor_status_;

protected:
    void setStatus(bool status);

public:
    explicit Sensor_Motion(std::string name);
};
