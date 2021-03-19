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

  void executeCharacteristicUpdate(CharacteristicIdentifier, int) override;

public:
  /**
   * constructor for class SH_Sensor_Temperature
   * @param gadget
   */
  explicit SH_Sensor_Temperature(std::string name);

  /**
   * method to return humidity
   * @return hummidity
   */
  int getHumidity() const;

  /**
   * method returns temperature
   * @return temperature
   */
  int getTemperature() const;

  vector<GadgetCharacteristicSettings> getCharacteristics() override;

  void handleMethodUpdate(GadgetMethod) override;
};