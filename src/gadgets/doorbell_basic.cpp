#include "doorbell_basic.h"

#include <utility>
#include "../hardware_controller.h"

/**
 * Constructor for the Doorbell_Basic
 * @param name Name of the gadget
 * @param pin Pin the doorbell switch is connected to
 * @param default_state The default state of the pin
 */
Doorbell_Basic::Doorbell_Basic(std::string name, uint8_t pin, bool default_state) :
    Doorbell(std::move(name)),
    switch_pin_(pin),
    default_state_(default_state),
    last_event_(0) {
  HardwareController::setPinMode(switch_pin_, INPUT);
};

void Doorbell_Basic::refresh() {
  if (HardwareController::digitalReadPin(switch_pin_) != default_state_) {
    unsigned long now = HardwareController::getMillis();
    if (now > last_event_ + HW_BOUNCE_DELAY) {
      doorbellTriggered();
    }
    last_event_ = now;
  }
}

void Doorbell_Basic::executeCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier characteristic, uint16_t step_value) {}
