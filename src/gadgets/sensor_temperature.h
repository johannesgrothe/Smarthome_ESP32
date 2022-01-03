#pragma once

#include "gadget.h"

class Sensor_Temperature : public Gadget {
protected:

  void executeCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier, uint16_t) override;

  void setTemperature(int temp);

  void setHumidity(int hum);

public:
  /**
   * constructor for class Sensor_Temperature
   * @param name Name for the gadget
   */
  explicit Sensor_Temperature(std::string name);
};