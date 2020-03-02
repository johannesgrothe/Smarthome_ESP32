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
    SH_Gadget(gadget) {
    setHomebridgeServiceType("Doorbell");
  };

  void triggerEvent() {
    has_changed = true;
//    updateHomebridgeCharacteristic("ProgrammableSwitchEvent", false);
    updateHomebridgeCharacteristic("ProgrammableSwitchEvent", true);
  }

  void print() override {};

//  Homebridge-Connector
  void applyHomebridgeCommand(const char *characteristic, int value) override {};

  bool getHomebridgeCharacteristics(char *buffer) override {return false;};
  // End of Homebridge-Connector

  // Code-Connector
  virtual void applyMappingMethod(const char *method) override {
    if (method != nullptr) {
      if (strcmp(method, "triggerDoorbell") == 0) {
        logger.addln("'triggerDoorbell'");
        triggerEvent();
      }
    } else {
      logger.addln(" - ");
    }
  };
  // End of Code-Connector
};

#endif //__SH_Doorbell__