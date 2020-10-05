#include "sh_sensor_motion.h"

SH_Sensor_Motion::SH_Sensor_Motion(const JsonObject &gadget) :
    SH_Gadget(gadget, GadgetType::Motion_Sensor),
    sensor_status_(false) {

}

void SH_Sensor_Motion::setStatus(bool status) {
  if (status != sensor_status_) {
      updateCharacteristic("status", (int) status);
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


void SH_Sensor_Motion::handleCharacteristicUpdate(const char *characteristic, int value) {
  logger.print(getName(), "Updating Characteristic: '");
  logger.print(characteristic);
  logger.println("'");
  if (strcmp(characteristic, "sensor_status") == 0) {
    setStatus(value);
  }
}

bool SH_Sensor_Motion::getCharacteristics(char *buffer) {
  sprintf(buffer, R"( "characteristics": {"sensor_status": %b})", sensor_status_);
  return true;
}
// End of Homebridge-Connector

// Code-Connector
void SH_Sensor_Motion::handleMethodUpdate(const char *) {
}

