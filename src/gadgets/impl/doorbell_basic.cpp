#include "doorbell_basic.h"

#include <hardware_controller.h>
#include <utility>
#include "../hardware_controller.h"

/**
 * Constructor for the Doorbell_Basic
 * @param name Name of the gadget
 * @param pin Pin the doorbell switch is connected to
 * @param default_state The default state of the pin
 */
Doorbell_Basic::Doorbell_Basic(const std::string &name, const uint8_t pin,
                               const bool default_state) : Doorbell(name),
                                                           switch_pin_(pin),
                                                           default_state_(default_state),
                                                           last_event_(0) {
    HardwareController::setPinMode(switch_pin_, INPUT);
};

void Doorbell_Basic::refresh() {
    if (HardwareController::digitalReadPin(switch_pin_) != default_state_) {
        const unsigned long now = HardwareController::getMillis();
        if (now > last_event_ + HW_BOUNCE_DELAY) {
            doorbellTriggered();
        }
        last_event_ = now;
    }
}
