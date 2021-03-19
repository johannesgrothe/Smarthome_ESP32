#include "sh_fan.h"

#include <utility>

SH_Fan::SH_Fan(std::string name, uint8_t levels_count) :
  SH_Gadget(std::move(name), GadgetType::Fan),
  rotation_speed_(0),
  last_rotation_speed_(0),
  levels_(levels_count) {}

void SH_Fan::toggleStatus() {
  setStatus(!getStatus());
}

bool SH_Fan::getStatus() const {
  return rotation_speed_ > 0;
}

byte SH_Fan::getLevel() const {
  return (rotation_speed_ / (FAN_ROTATION_SPEED_MAX / levels_));
}

void SH_Fan::setStatus(bool status) {
  if (status) {
    if (rotation_speed_ == 0) {
      rotation_speed_ = last_rotation_speed_;
    }
    updateCharacteristic(CharacteristicIdentifier::fanSpeed, rotation_speed_);
  } else {
    last_rotation_speed_ = rotation_speed_;
    rotation_speed_ = 0;
    updateCharacteristic(CharacteristicIdentifier::fanSpeed, rotation_speed_);
  }
  setGadgetHasChanged();
}

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
    updateCharacteristic(CharacteristicIdentifier::fanSpeed, rotation_speed_);
  } else {
    rotation_speed_ = new_speed;
    updateCharacteristic(CharacteristicIdentifier::fanSpeed, rotation_speed_);
  }
  setGadgetHasChanged();
}

void SH_Fan::executeCharacteristicUpdate(CharacteristicIdentifier characteristic, int value) {
  switch(characteristic) {
    case CharacteristicIdentifier::status:
      setStatus((bool) value);
      break;
    case CharacteristicIdentifier::fanSpeed:
      setRotationSpeed(value);
      break;
    default:
      break;
  }
}

vector<GadgetCharacteristicSettings> SH_Fan::getCharacteristics() {
  byte steps = (FAN_ROTATION_SPEED_MAX / levels_);
  return {GadgetCharacteristicSettings(CharacteristicIdentifier::status,
                                       0,
                                       1,
                                       1,
                                       int(rotation_speed_ > 0)),
          GadgetCharacteristicSettings(CharacteristicIdentifier::fanSpeed,
                                       0,
                                       FAN_ROTATION_SPEED_MAX,
                                       steps,
                                       int(rotation_speed_))};
}

void SH_Fan::handleMethodUpdate(GadgetMethod method) {
  switch (method) {
    case GadgetMethod::toggleStatus:
      toggleStatus();
      break;
    case GadgetMethod::turnOn:
      setStatus(true);
      break;
    case GadgetMethod::turnOff:
      setStatus(false);
      break;
    default:
      break;
  }
}
