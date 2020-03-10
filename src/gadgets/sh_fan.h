#ifndef __SH_FAN__
#define __SH_FAN__

#include "sh_gadget.h"

#define FAN_ROTATION_SPEED_MAX 100

class SH_Fan : public SH_Gadget {
protected:
  byte rotation_speed;
  byte last_rotation_speed;
  byte levels;

public:

  explicit SH_Fan(JsonObject gadget) :
    SH_Gadget(gadget),
    rotation_speed(0),
    last_rotation_speed(1) {
    setHomebridgeServiceType("Fan");
    if (gadget["levels"] != nullptr) {
      levels = (byte) gadget["levels"].as<unsigned int>();
    }
  };

  explicit SH_Fan(JsonObject gadget, byte levels_count) :
    SH_Gadget(gadget),
    rotation_speed(0),
    last_rotation_speed(1),
    levels(levels_count) {
    setHomebridgeServiceType("Fan");
  };

  // Status
  void toggleStatus() {
    setStatus(!getStatus());
  };

  bool getStatus() {
    return rotation_speed > 0;
  };

  byte getLevel() {
    return (rotation_speed / (FAN_ROTATION_SPEED_MAX / levels));
  }

  void setStatus(bool new_status, bool remote_update = true) {
    if (new_status) {
      if (rotation_speed == 0) {
        rotation_speed = last_rotation_speed;
      }
      updateHomebridgeCharacteristic("On", true, remote_update);
      updateHomebridgeCharacteristic("RotationSpeed", rotation_speed, remote_update);
    } else {
      last_rotation_speed = rotation_speed;
      rotation_speed = 0;
      updateHomebridgeCharacteristic("On", false, remote_update);
      updateHomebridgeCharacteristic("RotationSpeed", rotation_speed, remote_update);
    }
    has_changed = true;
  };

  byte getRotationSpeed() {
    return rotation_speed;
  }

  void rotationSpeedUp() {
    setRotationSpeed(getRotationSpeed() + (FAN_ROTATION_SPEED_MAX / levels));
  }

  void rotationSpeedDown() {
    setRotationSpeed(getRotationSpeed() - (FAN_ROTATION_SPEED_MAX / levels));
  }

  void setRotationSpeed(byte new_speed, bool remote_update = true) {
    if (new_speed > FAN_ROTATION_SPEED_MAX) new_speed = FAN_ROTATION_SPEED_MAX;
    if (new_speed == 0) {
      last_rotation_speed = rotation_speed;
      rotation_speed = 0;
      updateHomebridgeCharacteristic("On", true, remote_update);
      updateHomebridgeCharacteristic("RotationSpeed", rotation_speed, remote_update);
    } else {
      rotation_speed = new_speed;
      updateHomebridgeCharacteristic("On", true, remote_update);
      updateHomebridgeCharacteristic("RotationSpeed", rotation_speed, remote_update);
    }
    has_changed = true;
  }

  void print() override {
  }

//  Homebridge-Connector
  void applyHomebridgeCommand(const char *characteristic, int value) override {
    if (strcmp(characteristic, "On") == 0) {
      setStatus((bool) value, false);
    } else if (strcmp(characteristic, "RotationSpeed") == 0) {
      setRotationSpeed(value, false);
    }
  }

  bool getHomebridgeCharacteristics(char *buffer) override {
    byte steps = (FAN_ROTATION_SPEED_MAX / levels);
    sprintf(buffer, R"( "RotationSpeed": {"minValue": 0, "maxValue": %d, "minStep": %d})", FAN_ROTATION_SPEED_MAX, steps);
    return true;
  }
  // End of Homebridge-Connector

  // Code-Connector
  virtual void applyMappingMethod(const char *method) override {
    if (method != nullptr) {
      if (strcmp(method, "toggleStatus") == 0) {
        toggleStatus();
      } else if (strcmp(method, "'turnOn'") == 0) {
        setStatus(true);
      } else if (strcmp(method, "'turnOff'") == 0) {
        setStatus(false);
      }
    }
  }
  // End of Code-Connector
};

#endif //__SH_FAN__