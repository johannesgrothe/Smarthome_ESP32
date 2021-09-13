#include "sh_fan_westinghouse_ir.h"

SH_Fan_Westinghouse_IR::SH_Fan_Westinghouse_IR(std::string name) :
  SH_Fan(std::move(name), 3) {};

void SH_Fan_Westinghouse_IR::refresh() {
  if (gadgetHasChanged()) {
    byte level = getLevel();

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