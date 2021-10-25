#include "sh_sensor_motion.h"

#include <utility>

SH_Sensor_Motion::SH_Sensor_Motion(std::string name) :
    Gadget(std::move(name), GadgetType::Motion_Sensor, std::vector<Characteristic>()),
    sensor_status_(false) {}

void SH_Sensor_Motion::setStatus(bool status) {
  auto sensor_status = getCharacteristicValue(CharacteristicIdentifier::status);
  if (status != sensor_status) {
    setCharacteristicValue(CharacteristicIdentifier::status, (uint16_t) status);
    if (status) {
      logger_i(getName(), "Movement detected");
    } else {
      logger_i(getName(), "No further movement detected");
    }
  }
}

void SH_Sensor_Motion::executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) {
}
