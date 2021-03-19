#include "sh_sensor_motion.h"

#include <utility>

SH_Sensor_Motion::SH_Sensor_Motion(std::string name) :
    SH_Gadget(std::move(name), GadgetType::Motion_Sensor),
    sensor_status_(false) {}

void SH_Sensor_Motion::setStatus(bool status) {
  if (status != sensor_status_) {
    updateCharacteristic(CharacteristicIdentifier::status, (int) status);
    sensor_status_ = status;
  }
  if (status) {
    if (!sensor_status_){
      logger.println("movement detected!");
    }
  } else {
    if (sensor_status_) {
      logger.println("no further movement detected");
    }
  }
}

void SH_Sensor_Motion::executeCharacteristicUpdate(CharacteristicIdentifier characteristic, int value) {
  if (characteristic == CharacteristicIdentifier::status) {
    setStatus(value);
  }
}

vector<GadgetCharacteristicSettings> SH_Sensor_Motion::getCharacteristics() {
  return {GadgetCharacteristicSettings(CharacteristicIdentifier::status,
                                       0,
                                       1,
                                       1,
                                       int(sensor_status_))};
}

void SH_Sensor_Motion::handleMethodUpdate(GadgetMethod method) {}

