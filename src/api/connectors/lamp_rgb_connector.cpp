#include "lamp_rgb_connector.h"

LampRGBConnector::LampRGBConnector(Lamp_RGB& subject): GadgetAPIConector(subject) {

}

void LampRGBConnector::applyChanges(DynamicJsonDocument data) {
  }

DynamicJsonDocument LampRGBConnector::encode() {
  }