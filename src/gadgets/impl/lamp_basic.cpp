#include "lamp_basic.h"

#include <console_logger.h>
#include <hardware_controller.h>
#include <utility>

gadget_definitions::GadgetIdentifier Lamp_Basic::encodeType() {
    return gadget_definitions::GadgetIdentifier::lamp_basic;
}

Lamp_Basic::Lamp_Basic(std::string name, const uint8_t pin) : Lamp_OnOff(std::move(name)),
                                                              pin_(pin),
                                                              status_(false) {
}

void Lamp_Basic::refresh() {
    if (wasChanged()) {
        logger_i(getName(), "Was changed: %d", status_);
        HardwareController::digitalWritePin(pin_, status_);
    }
}
