#include "fan_westinghouse_ir.h"

#include <console_logger.h>

Fan_Westinghouse_IR::Fan_Westinghouse_IR(const std::string &name, const IR_Gadget &ir) : Fan(name, 3),
    ir_(ir) {
};

void Fan_Westinghouse_IR::refresh() {
    if (wasChanged()) {
        bool status = true;
        logger_i(getName(), "Has changed: %d", level);
        if (getLevel() == 0) {
            status = ir_.sendRawIR(level_0, 95);
        } else if (getLevel() == 1) {
            status = ir_.sendRawIR(level_1, 119);
        } else if (getLevel() == 2) {
            // TODO: level_2 code needed
            // sendRawIR(level_0, 95);
        } else if (getLevel() == 3) {
            status = ir_.sendRawIR(level_3, 95);
        }
        if (!status) {
            logger_e(getName(), "Could not send IR");
        }
    }
}
