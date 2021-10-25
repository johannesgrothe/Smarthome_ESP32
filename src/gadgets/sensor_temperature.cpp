#include "sensor_temperature.h"

#include <utility>

Sensor_Temperature::Sensor_Temperature(std::string name) :
    Gadget(std::move(name),
           GadgetType::Temp_Humm_Sensor,
           {
                  Characteristic(CharacteristicIdentifier::humidity,
                                 0,
                                 100,
                                 100),
                  Characteristic(CharacteristicIdentifier::temperature,
                                 (-60),
                                 80,
                                 140)
              }
              ) {}

void Sensor_Temperature::executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) {

}

void Sensor_Temperature::setTemperature(int temp) {
  // TODO: set
}

void Sensor_Temperature::setHumidity(int hum) {
  // TODO: set
}
