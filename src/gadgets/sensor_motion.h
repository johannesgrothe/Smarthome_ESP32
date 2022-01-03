#pragma once

#include "gadget.h"

class Sensor_Motion : public Gadget {
private:
  bool sensor_status_;

protected:
  /**
   * updates local attribute sensor_status, if necessary.
   * also updates Remote
   * @param status new status
   */
  void setStatus(bool);

  void executeCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier, uint16_t) override;

public:
  explicit Sensor_Motion(std::string name);
};