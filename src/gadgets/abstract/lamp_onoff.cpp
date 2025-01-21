#include "lamp_onoff.h"

#include <utility>

Lamp_OnOff::Lamp_OnOff(std::string name) : Gadget(std::move(name),
                                                  GadgetType::Lightbulb),
                                           status_(false) {
}

bool Lamp_OnOff::getStatus() const {
    return status_;
}

void Lamp_OnOff::setStatus(const bool status) {
    registerExternalChange();
    status_ = status;
}
