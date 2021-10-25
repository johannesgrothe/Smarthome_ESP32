#pragma once

#include "gadget.h"

class LampOnOff: public Gadget {
public:
  explicit LampOnOff(std::string name);

  /**
   * Checks if the lamp is on or off
   * @return True if the lamp is on, false if the lamp is off
   */
  bool getStatus();

  /**
   * Turns the lamp on or off
   * @param status True of lamp should be turned on, false if it should be turned off
   */
  void setStatus(bool status);
};