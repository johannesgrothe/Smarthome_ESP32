#pragma once

#include "sh_gadget.h"

class SH_Sensor_Motion : public SH_Gadget {
private:
  bool sensor_status_;

protected:
  /**
   * updates local attribute sensor_status, if necessary.
   * also updates Remote
   * @param status new status
   */
  void setStatus(bool);

public:
  /**
   * Constructor for class SH_Sensor_Motion
   * @param gadget
   */
  explicit SH_Sensor_Motion(std::string name);

  void handleCharacteristicUpdate(const char *, int) override;

  bool getCharacteristics(char *) override;

  void handleMethodUpdate(GadgetMethod) override;
};