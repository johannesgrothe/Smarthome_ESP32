#pragma once

#include "gadget.h"

class SH_Sensor_Temperature : public Gadget {
protected:

  void executeCharacteristicUpdate(CharacteristicIdentifier, uint16_t) override;

public:
  /**
   * constructor for class SH_Sensor_Temperature
   * @param gadget
   */
  explicit SH_Sensor_Temperature(std::string name);

};