#include "sh_doorbell.h"

SH_Doorbell::SH_Doorbell(const JsonObject& gadget) :
  SH_Gadget(gadget, GadgetType::Doorbell) {};

void SH_Doorbell::triggerEvent() {
  has_changed = true;
//    updateHomebridgeCharacteristic("ProgrammableSwitchEvent", false);
  updateCharacteristic("ProgrammableSwitchEvent", true);
}

void SH_Doorbell::print() {};

void SH_Doorbell::handleCharacteristicUpdate(const char *characteristic, int value) {
  logger.print(getName(), "Updating Characteristic: '");
  logger.add(characteristic);
  logger.addln("'");
}

bool SH_Doorbell::getCharacteristics(char *buffer) { return false; };

void SH_Doorbell::handleMethodUpdate(const char *method) {
  if (method != nullptr) {
    if (strcmp(method, "triggerDoorbell") == 0) {
      triggerEvent();
    }
  }
}