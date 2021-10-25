#pragma once

#include "gadget.h"

class Fan : public Gadget {
public:

  /**
   * Constructor for the fan
   * @param name Name of the gadget
   * @param levels_count How many levels the fan should operate at
   */
  Fan(const std::string& name, uint16_t levels_count);
};
