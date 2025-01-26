#include "wallswitch_basic.h"

#include "../../hardware_controller.h"

void Switch_Basic::refresh() {
    if (HardwareController::digitalReadPin(pin_) != default_state_) {
        const unsigned long now = HardwareController::getMillis();
        if (last_action_ + 50 < now || last_action_ > now) {
            switchTriggered();
        }
        last_action_ = now;
    }
}

gadget_definitions::GadgetIdentifier Switch_Basic::encodeType() {
    return gadget_definitions::GadgetIdentifier::wallswitch_basic;
}

Switch_Basic::Switch_Basic(std::string name, const uint8_t pin, const bool default_state) : Switch(std::move(name)),
    pin_(pin),
    last_action_(0),
    default_state_(default_state) {
    HardwareController::setPinMode(pin_, INPUT);
}
