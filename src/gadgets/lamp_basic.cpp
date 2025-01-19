#include "lamp_basic.h"

#include <console_logger.h>
#include <hardware_controller.h>
#include <utility>

Lamp_Basic::Lamp_Basic(std::string name, const uint8_t pin) : Lamp_OnOff(std::move(name)),
                                                              SimpleHardwareGadget(true),
                                                              pin_(pin),
                                                              status_(false) {
}

void Lamp_Basic::refresh() {
    if (executeHWChange()) {
        logger_i(getName(), "Has changed: %d", status_);
        HardwareController::digitalWritePin(pin_, status_);
    }
}
