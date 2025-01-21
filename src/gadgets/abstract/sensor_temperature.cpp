#include "sensor_temperature.h"

#include <utility>

Sensor_Temperature::Sensor_Temperature(std::string name) : Gadget(std::move(name),
                                                                  GadgetType::Temp_Humm_Sensor),
                                                           temp_(0),
                                                           humidity_(0) {
}


void Sensor_Temperature::setTemperature(const int temp) {
    registerInternalChange();
    temp_ = temp;
}

void Sensor_Temperature::setHumidity(const int hum) {
    registerInternalChange();
    humidity_ = hum;
}
