#ifndef __SH_Doorbell__
#define __SH_Doorbell__

#include "sh_gadget.h"

class SH_Doorbell : public SH_Gadget {
protected:
  float lightness;
  float last_lightness{};
  float default_lightness;
  float min_lightness;

  float saturation;

  float hue;

  SH_LAMP_TYPE type;

public:

  explicit SH_Doorbell(JsonObject gadget) :
    SH_Gadget(gadget, Doorbell) {};

  void triggerEvent() {
    has_changed = true;
//    updateHomebridgeCharacteristic("ProgrammableSwitchEvent", false);
    updateCharacteristic("ProgrammableSwitchEvent", true);
  }

  void print() override {};

//  Homebridge-Connector
  void handleCharacteristicUpdate(const char *characteristic, int value) override {
    logger.print(getName(), "Updating Characteristic: '");
    logger.add(characteristic);
    logger.addln("'");
  };

  bool getCharacteristics(char *buffer) override {return false;};
  // End of Homebridge-Connector

  // Code-Connector
  virtual void handleMethodUpdate(const char *method) override {
    if (method != nullptr) {
      if (strcmp(method, "triggerDoorbell") == 0) {
        triggerEvent();
      }
    }
  };
  // End of Code-Connector
};

#endif //__SH_Doorbell__