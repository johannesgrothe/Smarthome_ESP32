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
  explicit SH_Sensor_Motion(const JsonObject &gadget);

  void print() override;

  //  Homebridge-Connector
  void handleCharacteristicUpdate(const char *, int) override;

  bool getCharacteristics(char *) override;
  // End of Homebridge-Connector

  // Code-Connector
  void handleMethodUpdate(const char *) override;
  // End of Code-Connector
};