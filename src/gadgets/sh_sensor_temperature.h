#pragma once

#include "sh_gadget.h"

class SH_Sensor_Temperature : public SH_Gadget {
private:
  int humidity_;
  int temperature_;

protected:
  /**
   * method sets the Humidity and updates Remote
   * @param hummiditiy
   */
  void setHumidity(int new_humiditiy);

  /**
   * method sets the Temperature and updates Remote
   * @param temperature
   */
  void setTemperature(int new_temperature);

public:
  /**
   * constructor for class SH_Sensor_Temperature
   * @param gadget
   */
  explicit SH_Sensor_Temperature(const JsonObject &gadget);

  /**
   * method to return humidity
   * @return hummidity
   */
  int getHumidity();

  /**
   * method returns temperature
   * @return temperature
   */
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