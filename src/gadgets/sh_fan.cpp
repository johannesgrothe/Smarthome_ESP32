#include "sh_fan.h"

SH_Fan::SH_Fan(const JsonObject& gadget) :
  SH_Gadget(gadget, GadgetType::Fan),
  rotation_speed_(0),
  last_rotation_speed_(1) {
  if (gadget["levels"] != nullptr) {
    levels_ = (byte) gadget["levels"].as<unsigned int>();
  }
};

SH_Fan::SH_Fan(const JsonObject& gadget, byte levels_count) :
  SH_Gadget(gadget, GadgetType::Fan),
  rotation_speed_(0),
  last_rotation_speed_(1),
  levels_(levels_count) {};

// Status
void SH_Fan::toggleStatus() {
  setStatus(!getStatus());
};

bool SH_Fan::getStatus() const {
  return rotation_speed_ > 0;
};

byte SH_Fan::getLevel() const {
  return (rotation_speed_ / (FAN_ROTATION_SPEED_MAX / levels_));
}

void SH_Fan::setStatus(bool new_status) {
  if (new_status) {
    if (rotation_speed_ == 0) {
      rotation_speed_ = last_rotation_speed_;
    }
    updateCharacteristic("rotationspeed", rotation_speed_);
  } else {
    last_rotation_speed_ = rotation_speed_;
    rotation_speed_ = 0;
    updateCharacteristic("rotationspeed", rotation_speed_);
  }
  has_changed = true;
};

byte SH_Fan::getRotationSpeed() const {
  return rotation_speed_;
}

void SH_Fan::rotationSpeedUp() {
  setRotationSpeed(getRotationSpeed() + (FAN_ROTATION_SPEED_MAX / levels_));
}

void SH_Fan::rotationSpeedDown() {
  setRotationSpeed(getRotationSpeed() - (FAN_ROTATION_SPEED_MAX / levels_));
}

void SH_Fan::setRotationSpeed(byte new_speed) {
  if (new_speed > FAN_ROTATION_SPEED_MAX) new_speed = FAN_ROTATION_SPEED_MAX;
  if (new_speed == 0) {
    last_rotation_speed_ = rotation_speed_;
    rotation_speed_ = 0;
    updateCharacteristic("rotaionspeed", rotation_speed_);
  } else {
    rotation_speed_ = new_speed;
    updateCharacteristic("rotaionspeed", rotation_speed_);
  }
  has_changed = true;
}

void SH_Fan::print() {
}

void SH_Fan::handleCharacteristicUpdate(const char *characteristic, int value) {
  logger.print(getName(), "Updating Characteristic: '");
  logger.add(characteristic);
  logger.addln("'");
  if (strcmp(characteristic, "On") == 0) {
    setStatus((bool) value);
  } else if (strcmp(characteristic, "rotationspeed") == 0) {
    setRotationSpeed(value);
  }
}

bool SH_Fan::getCharacteristics(char *buffer) {
  byte steps = (FAN_ROTATION_SPEED_MAX / levels_);
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