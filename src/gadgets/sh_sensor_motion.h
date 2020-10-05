#pragma once

#include "sh_gadget.h"

class SH_Sensor_Motion : public SH_Gadget {
private:
  bool sensor_status_;

protected:
  void setStatus(bool);

public:
  explicit SH_Sensor_Motion(const JsonObject&);

  void print();

  //  Homebridge-Connector
  void handleCharacteristicUpdate(const char *, int) override;

  bool getCharacteristics(char *) override;
  // End of Homebridge-Connector

  // Code-Connector
  void handleMethodUpdate(const char *) override;
  // End of Code-Connector
};