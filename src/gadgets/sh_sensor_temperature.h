#pragma once

#include "sh_gadget.h"

class SH_Sensor_Temperature : public SH_Gadget {
private:
  int humidity_;
  int temperature_;

protected:
  void setHumidity(int);
  void setTemperature(int);

public:
  explicit SH_Sensor_Temperature(const JsonObject&);

  int getHummidity();

  int getTemperature();

  void print() override;

//  Homebridge-Connector
  void handleCharacteristicUpdate(const char *, int) override;

  bool getCharacteristics(char *) override;
  // End of Homebridge-Connector

  // Code-Connector
  void handleMethodUpdate(const char *) override;
  // End of Code-Connector
};