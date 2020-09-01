#include "sh_fan.h"

SH_Fan::SH_Fan(const JsonObject& gadget) :
  SH_Gadget(gadget, Fan),
  rotation_speed(0),
  last_rotation_speed(1) {
  if (gadget["levels"] != nullptr) {
    levels = (byte) gadget["levels"].as<unsigned int>();
  }
};

SH_Fan::SH_Fan(const JsonObject& gadget, byte levels_count) :
  SH_Gadget(gadget, Fan),
  rotation_speed(0),
  last_rotation_speed(1),
  levels(levels_count) {};

// Status
void SH_Fan::toggleStatus() {
  setStatus(!getStatus());
};

bool SH_Fan::getStatus() const {
  return rotation_speed > 0;
};

byte SH_Fan::getLevel() const {
  return (rotation_speed / (FAN_ROTATION_SPEED_MAX / levels));
}

void SH_Fan::setStatus(bool new_status) {
  if (new_status) {
    if (rotation_speed == 0) {
      rotation_speed = last_rotation_speed;
    }
    updateCharacteristic("rotationspeed", rotation_speed);
  } else {
    last_rotation_speed = rotation_speed;
    rotation_speed = 0;
    updateCharacteristic("rotationspeed", rotation_speed);
  }
  has_changed = true;
};

byte SH_Fan::getRotationSpeed() const {
  return rotation_speed;
}

void SH_Fan::rotationSpeedUp() {
  setRotationSpeed(getRotationSpeed() + (FAN_ROTATION_SPEED_MAX / levels));
}

void SH_Fan::rotationSpeedDown() {
  setRotationSpeed(getRotationSpeed() - (FAN_ROTATION_SPEED_MAX / levels));
}

void SH_Fan::setRotationSpeed(byte new_speed) {
  if (new_speed > FAN_ROTATION_SPEED_MAX) new_speed = FAN_ROTATION_SPEED_MAX;
  if (new_speed == 0) {
    last_rotation_speed = rotation_speed;
    rotation_speed = 0;
    updateCharacteristic("rotaionspeed", rotation_speed);
  } else {
    rotation_speed = new_speed;
    updateCharacteristic("rotaionspeed", rotation_speed);
  }
  has_changed = true;
}

void SH_Fan::print() {
}

void SH_Fan::handleCharacteristicUpdate(const char *characteristic, int value) {
  logger.print(getName(), "Updating Characteristic: '");
  logger.print(characteristic);
  logger.println("'");
  if (strcmp(characteristic, "On") == 0) {
    setStatus((bool) value);
  } else if (strcmp(characteristic, "rotationspeed") == 0) {
    setRotationSpeed(value);
  }
}

bool SH_Fan::getCharacteristics(char *buffer) {
  byte steps = (FAN_ROTATION_SPEED_MAX / levels);
  sprintf(buffer, R"( "rotationspeed": {"min": 0, "max": %d, "step": %d})", FAN_ROTATION_SPEED_MAX,
          steps);
  return true;
}

void SH_Fan::handleMethodUpdate(const char *method) {
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