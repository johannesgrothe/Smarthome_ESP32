#pragma once

#include "sh_gadget.h"

class SH_Doorbell : public SH_Gadget {
protected:

public:

  explicit SH_Doorbell(const JsonObject& gadget);

  void triggerEvent();

  void print() override;

//  Homebridge-Connector
  void handleCharacteristicUpdate(const char *characteristic, int value) override;

  bool getCharacteristics(char *buffer) override;
  // End of Homebridge-Connector

  // Code-Connector
  void handleMethodUpdate(const char *method) override;
  // End of Code-Connector
};
