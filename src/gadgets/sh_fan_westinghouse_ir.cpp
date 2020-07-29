#include "sh_fan_westinghouse_ir.h"

SH_Fan_Westinghouse_IR::SH_Fan_Westinghouse_IR(const JsonObject& gadget) :
  SH_Fan(gadget, 3) {};

void SH_Fan_Westinghouse_IR::refresh() {
  if (has_changed) {
    byte level = getLevel();
    logger.print(getName(), "has changed: Level ");
    logger.addln(level);
    logger.incIndent();
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
    logger.decIndent();
  }
  has_changed = false;
}