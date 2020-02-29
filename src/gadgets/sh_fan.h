#ifndef __SH_FAN__
#define __SH_FAN__

#include "sh_gadget.h"

class SH_Fan : public SH_Gadget {
protected:
  byte rotation_speed;
  byte last_rotation_speed;

public:

  explicit SH_Fan(JsonObject gadget) :
    SH_Gadget(gadget),
    rotation_speed(0),
    last_rotation_speed(1) {
    setHomebridgeServiceType("Fan");
  };

  // Status
  void toggleStatus() {
    setStatus(!getStatus());
  };

  bool getStatus() {
    return rotation_speed > 0;
  };

  void setStatus(bool new_status, bool remote_update=true) {
    if (new_status) {
      updateHomebridgeCharacteristic("On", true, remote_update);
      setRotationSpeed(last_rotation_speed, false);
    } else {
      setRotationSpeed(0, false);
      updateHomebridgeCharacteristic("On", false, remote_update);
    }
    has_changed = true;
//    updateHomebridgeCharacteristic("On", new_status);
  };

  byte getRotationSpeed() {
    return rotation_speed;
  }

  void setRotationSpeed(byte new_speed, bool remote_update=true) {
    if (new_speed == 0) {
      last_rotation_speed = rotation_speed;
      rotation_speed = new_speed;
      updateHomebridgeCharacteristic("RotationSpeed", rotation_speed, remote_update);
    } else {
      rotation_speed = new_speed;
      updateHomebridgeCharacteristic("RotationSpeed", rotation_speed, remote_update);
    }
    has_changed = true;
  }

  void print() override {
  }

//  Homebridge-Connector
  void applyHomebridgeCommand(const char *characteristic, int value) override {
    if (strcmp(characteristic, "On") == 0) {
      setStatus((bool) value);
    } else if (strcmp(characteristic, "RotationSpeed") == 0) {
      setStatus(value);
    }
  }

  bool getHomebridgeCharacteristics(char *buffer) override {
    strcpy(buffer,  R"( "RotationSpeed": {"minValue": 0, "maxValue": 100, "minStep": 25})");
    return true;
  }
  // End of Homebridge-Connector

  // Code-Connector
  virtual void applyMappingMethod(const char *method) override {
    if (method != nullptr) {
      if (strcmp(method, "toggleStatus") == 0) {
        logger.addln("'toggleStatus'");
        toggleStatus();
      } else if (strcmp(method, "'turnOn'") == 0) {
        logger.addln("'turnOn");
        setStatus(true);
      } else if (strcmp(method, "'turnOff'") == 0) {
        logger.addln("'turnOff");
        setStatus(false);
      } else {
        logger.add("Found Nothing for '");
        logger.add(method);
        logger.addln("'");
      }
    } else {
      logger.addln(" - ");
    }
  }
  // End of Code-Connector
};

#endif //__SH_FAN__