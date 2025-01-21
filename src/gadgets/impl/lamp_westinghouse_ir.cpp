#include "lamp_westinghouse_ir.h"

#include <utility>

Lamp_WestinghouseIR::Lamp_WestinghouseIR(std::string name, const IR_Gadget &ir) : Lamp_OnOff(std::move(name)),
    ir_(ir) {
}

void Lamp_WestinghouseIR::refresh() {
    if (wasChanged()) {
        bool status;
        if (getStatus())
            status = ir_.sendRawIR(lamp_on, 143);
        else
            status = ir_.sendRawIR(lamp_off, 119);
        if (!status) {
            logger_e(getName(), "Could not send IR");
        }
    }
}
