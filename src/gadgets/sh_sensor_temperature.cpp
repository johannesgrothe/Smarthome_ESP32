#include "sh_sensor_temperature.h"

SH_Sensor_Temperature::SH_Sensor_Temperature(std::string name) :
  SH_Gadget(name, GadgetType::Temp_Humm_Sensor),
  humidity_(0),
  temperature_(0) {}


void SH_Sensor_Temperature::setHumidity(int new_humiditiy) {
  if (new_humiditiy != humidity_) {
    humidity_ = new_humiditiy;
    updateCharacteristic("humidity", new_humiditiy);
  }
}

void SH_Sensor_Temperature::setTemperature(int new_temperature) {
  if (new_temperature != temperature_) {
    temperature_ = new_temperature;
    updateCharacteristic("temperature", new_temperature);
  }
}

int SH_Sensor_Temperature::getHumidity() {
  return humidity_;
}

int SH_Sensor_Temperature::getTemperature() {
  return temperature_;
}

void SH_Sensor_Temperature::handleCharacteristicUpdate(const char *characteristic, int value) {
  logger.print(getName(), "Updating Characteristic: '");
  logger.print(characteristic);
  logger.println("'");
  if (strcmp(characteristic, "Hummidity") == 0) {
    setHumidity(value);
  } else if (strcmp(characteristic, "Temperature") == 0) {
    setTemperature(value);
  }
}

bool SH_Sensor_Temperature::getCharacteristics(char *buffer) {
  sprintf(buffer, R"( "characteristics": {"hummidity": %d, "temperature": %d}})", humidity_, temperature_);
  return true;
}

void SH_Sensor_Temperature::handleMethodUpdate(GadgetMethod) {}

