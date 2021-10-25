#pragma once

#include "gadget.h"

class SH_Sensor_Motion : public Gadget {
private:
  bool sensor_status_;

protected:
  /**
   * updates local attribute sensor_status, if necessary.
   * also updates Remote
   * @param status new status
   */
  void setStatus(bool);

  void executeCharacteristicUpdate(CharacteristicIdentifier, uint16_t) override;

public:
  explicit SH_Sensor_Motion(std::string name);
};