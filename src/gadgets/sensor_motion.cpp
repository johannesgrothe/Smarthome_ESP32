#include "sensor_motion.h"

#include <utility>

Sensor_Motion::Sensor_Motion(std::string name) :
    Gadget(std::move(name), GadgetType::Motion_Sensor, std::vector<Characteristic>()),
    sensor_status_(false) {}

void Sensor_Motion::setStatus(bool status) {
  auto sensor_status = getCharacteristicValue(gadget_definitions::CharacteristicIdentifier::status);
  if (status != sensor_status) {
    setCharacteristicValue(gadget_definitions::CharacteristicIdentifier::status, (uint16_t) status);
    if (status) {
      logger_i(getName(), "Movement detected");
    } else {
      logger_i(getName(), "No further movement detected");
    }
  }
}

void Sensor_Motion::executeCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier characteristic, uint16_t step_value) {
}
