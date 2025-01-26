#pragma once

#include "../abstract/sensor_motion.h"

class Sensor_Motion_HR501 final : public Sensor_Motion {
    int input_pin_;

protected:
    gadget_definitions::GadgetIdentifier encodeType() override;

public:
    explicit Sensor_Motion_HR501(std::string name, uint8_t pin);

    void refresh() override;
};
