#include "sensor_motion.h"

#include <console_logger.h>
#include <utility>

Sensor_Motion::Sensor_Motion(std::string name) : Gadget(std::move(name), GadgetType::Motion_Sensor),
                                                 sensor_status_(false) {
}

void Sensor_Motion::setStatus(const bool status) {
    if (sensor_status_ != status) {
        registerInternalChange();
        sensor_status_ = status;
    }
}
