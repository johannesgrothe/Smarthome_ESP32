#include "gadget_remote.h"

bool
GadgetRemote::registerGadget(const char *gadget_name, GadgetType gadget_type,
                             const char *characteristics) { return false; };

bool GadgetRemote::removeGadget(const char *gadget_name) { return false; };

bool
GadgetRemote::registerGadgetOnRemote(const char *gadget_name, GadgetType gadget_type, const char *characteristics) {
  logger.println("Registering Gadget:");
  logger.incIndent();
  if (registerGadget(gadget_name, gadget_type, characteristics)) {
    logger.println(LOG_TYPE::INFO, "OK");
    logger.decIndent();
    return true;
  } else {
    logger.println(LOG_TYPE::ERR, "ERR");
    logger.decIndent();
  }
  return false;
}

bool GadgetRemote::handleNewGadget(SH_Gadget *new_gadget) {
  char characteristic_str[HOMEBRIDGE_REGISTER_STR_MAX_LEN - 60];
  new_gadget->
    getCharacteristics(&characteristic_str[0]);
  if (registerGadgetOnRemote(new_gadget->getName(), new_gadget->getType(), characteristic_str))
    logger.println("Registering Gadget successfull.");
  else
    logger.println(LOG_TYPE::ERR, "Failed to register Gadget.");
  return true;
}

GadgetRemote::GadgetRemote(JsonObject data) :
  Remote(data) {};

GadgetRemote::GadgetRemote(Request_Gadget *gadget, JsonObject data) :
  Remote(gadget, data) {};

void
GadgetRemote::updateCharacteristic(const char *gadget_name, const char *service, const char *characteristic,
                                   int value) {};