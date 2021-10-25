#include "sh_sensor_temperature.h"

#include <utility>

SH_Sensor_Temperature::SH_Sensor_Temperature(std::string name) :
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

void SH_Sensor_Temperature::executeCharacteristicUpdate(CharacteristicIdentifier characteristic, uint16_t step_value) {

}
