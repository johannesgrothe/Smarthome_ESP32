#include "sh_sensor_temperature.h"

#include <utility>

SH_Sensor_Temperature::SH_Sensor_Temperature(std::string name) :
  SH_Gadget(std::move(name), GadgetType::Temp_Humm_Sensor),
  humidity_(0),
  temperature_(0) {}

void SH_Sensor_Temperature::setHumidity(int new_humiditiy) {
  if (new_humiditiy != humidity_) {
    humidity_ = new_humiditiy;
    updateCharacteristic(CharacteristicIdentifier::humidity, new_humiditiy);
  }
}

void SH_Sensor_Temperature::setTemperature(int new_temperature) {
  if (new_temperature != temperature_) {
    temperature_ = new_temperature;
    updateCharacteristic(CharacteristicIdentifier::humidity, new_temperature);
  }
}

int SH_Sensor_Temperature::getHumidity() const {
  return humidity_;
}

int SH_Sensor_Temperature::getTemperature() const {
  return temperature_;
}

void SH_Sensor_Temperature::executeCharacteristicUpdate(CharacteristicIdentifier characteristic, int value) {
  switch (characteristic) {
    case CharacteristicIdentifier::humidity:
      setHumidity(value);
      break;
    case CharacteristicIdentifier::temperature:
      setTemperature(value);
      break;
    default:
      break;
  }
}

vector<GadgetCharacteristicSettings> SH_Sensor_Temperature::getCharacteristics() {
  return {GadgetCharacteristicSettings(CharacteristicIdentifier::humidity,
                                       0,
                                       100,
                                       1,
                                       int(humidity_)),
          GadgetCharacteristicSettings(CharacteristicIdentifier::temperature,
                                       -50,
                                       100,
                                       1,
                                       int(temperature_))};
}

void SH_Sensor_Temperature::handleMethodUpdate(GadgetMethod) {}

