#pragma once

#include "gadget.h"
#include "../color.h"

class Lamp_RGB : public Gadget {
public:

  /**
   * Constructor for the Lamp_RGB
   * @param name Name of the gadget
   */
  explicit Lamp_RGB(std::string name);
};
