#include "sh_fan.h"

#include <utility>

SH_Fan::SH_Fan(std::string name, uint8_t levels_count) :
  SH_Gadget(std::move(name), GadgetType::Fan),
  rotation_speed_(0),
  last_rotation_speed_(0),
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

void SH_Fan::setStatus(bool status) {
  if (status) {
    if (rotation_speed_ == 0) {
      rotation_speed_ = last_rotation_speed_;
    }
    updateCharacteristic("rotationspeed", rotation_speed_);
  } else {
    last_rotation_speed_ = rotation_speed_;
    rotation_speed_ = 0;
    updateCharacteristic("rotationspeed", rotation_speed_);
  }
  setGadgetHasChanged();
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
  setGadgetHasChanged();
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