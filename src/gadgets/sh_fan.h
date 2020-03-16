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
    SH_Gadget(gadget, Fan),
    rotation_speed(0),
    last_rotation_speed(1) {
    if (gadget["levels"] != nullptr) {
      levels = (byte) gadget["levels"].as<unsigned int>();
    }
  };

  explicit SH_Fan(JsonObject gadget, byte levels_count) :
    SH_Gadget(gadget, Fan),
    rotation_speed(0),
    last_rotation_speed(1),
    levels(levels_count) {};

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

  void setStatus(bool new_status) {
    if (new_status) {
      if (rotation_speed == 0) {
        rotation_speed = last_rotation_speed;
      }
      updateCharacteristic("On", true);
      updateCharacteristic("RotationSpeed", rotation_speed);
    } else {
      last_rotation_speed = rotation_speed;
      rotation_speed = 0;
      updateCharacteristic("On", false);
      updateCharacteristic("RotationSpeed", rotation_speed);
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

  void setRotationSpeed(byte new_speed) {
    if (new_speed > FAN_ROTATION_SPEED_MAX) new_speed = FAN_ROTATION_SPEED_MAX;
    if (new_speed == 0) {
      last_rotation_speed = rotation_speed;
      rotation_speed = 0;
      updateCharacteristic("On", true);
      updateCharacteristic("RotationSpeed", rotation_speed);
    } else {
      rotation_speed = new_speed;
      updateCharacteristic("On", true);
      updateCharacteristic("RotationSpeed", rotation_speed);
    }
    has_changed = true;
  }

  void print() override {
  }

//  Homebridge-Connector
  void handleCharacteristicUpdate(const char *characteristic, int value) override {
    logger.print(getName(), "Updating Characteristic: '");
    logger.add(characteristic);
    logger.addln("'");
    if (strcmp(characteristic, "On") == 0) {
      setStatus((bool) value);
    } else if (strcmp(characteristic, "RotationSpeed") == 0) {
      setRotationSpeed(value);
    }
  }

  bool getCharacteristics(char *buffer) override {
    byte steps = (FAN_ROTATION_SPEED_MAX / levels);
    sprintf(buffer, R"( "RotationSpeed": {"minValue": 0, "maxValue": %d, "minStep": %d})", FAN_ROTATION_SPEED_MAX,
            steps);
    return true;
  }
  // End of Homebridge-Connector

  // Code-Connector
  virtual void handleMethodUpdate(const char *method) override {
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