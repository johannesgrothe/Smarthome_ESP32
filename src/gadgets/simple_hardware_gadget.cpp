#include "simple_hardware_gadget.h"

SimpleHardwareGadget::SimpleHardwareGadget(bool needs_initial_update) :
    needs_hw_update_(needs_initial_update) {}

void SimpleHardwareGadget::setHWChangeStatus(bool needs_hw_change) {
  needs_hw_update_ = needs_hw_change;
}

bool SimpleHardwareGadget::executeHWChange() {
  auto buf_status = needs_hw_update_;
  needs_hw_update_ = false;
  return buf_status;
}

bool SimpleHardwareGadget::needsHWChange() const {
  return needs_hw_update_;
}
