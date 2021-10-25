#pragma once

#include "gadget.h"
#include "simple_hardware_gadget.h"

#define FAN_ROTATION_SPEED_MAX 100

class SH_Fan : public Gadget, public SimpleHardwareGadget {
protected:

  void executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) override;

public:

  /**
   * Constructor for the fan
   * @param name Name of the gadget
   * @param levels_count How many levels the fan should operate at
   */
  SH_Fan(const std::string& name, uint16_t levels_count);
};
