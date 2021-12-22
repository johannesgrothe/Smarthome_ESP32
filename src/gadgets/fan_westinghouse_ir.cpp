#include "fan_westinghouse_ir.h"

Fan_Westinghouse_IR::Fan_Westinghouse_IR(const std::string &name) :
    Fan(name, 3),
    SimpleHardwareGadget(true) {};

void Fan_Westinghouse_IR::refresh() {
  if (executeHWChange()) {
    auto level = getCharacteristicValue(CharacteristicIdentifier::fan_speed);

    logger_i(getName(), "Has changed: %d", level);
    if (level == 0) {
      sendRawIR(level_0, 95);
    } else if (level == 1) {
      sendRawIR(level_1, 119);
    } else if (level == 2) {
      // TODO: level_2 code needed
      // sendRawIR(level_0, 95);
    } else if (level == 3) {
      sendRawIR(level_3, 95);
    }
  }
}

void Fan_Westinghouse_IR::executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) {
  setHWChangeStatus(true);
}
