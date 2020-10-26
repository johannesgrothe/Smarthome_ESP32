#include "sh_lamp_basic.h"

#include <utility>

SH_Lamp_Basic::SH_Lamp_Basic(std::string name, uint8_t pin) :
  SH_Lamp(std::move(name), SHLampType::ON_OFF),
  pin_(pin) {}

void SH_Lamp_Basic::refresh() {
  if (gadgetHasChanged()) {
    logger.println(getName(), "has changed.");
    digitalWrite(pin_, getStatus());
  }
}
