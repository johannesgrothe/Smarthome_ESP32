#include "gadget_remote.h"

#include <utility>

bool
GadgetRemote::registerGadgetOnRemote(const string& gadget_name, GadgetType gadget_type, vector<GadgetCharacteristic> characteristics) {
  logger.println("Registering Gadget:");
  logger.incIndent();
  if (registerGadget(gadget_name, gadget_type, std::move(characteristics))) {
    logger.println(LOG_TYPE::INFO, "OK");
    logger.decIndent();
    return true;
  } else {
    logger.println(LOG_TYPE::ERR, "ERR");
    logger.decIndent();
  }
  return false;
}

bool GadgetRemote::handleNewGadget(std::shared_ptr<SH_Gadget> new_gadget) {
  if (registerGadgetOnRemote(new_gadget->getName(),
                             new_gadget->getType(),
                             new_gadget->getCharacteristics()))
    logger.println("Registering Gadget successfull.");
  else
    logger.println(LOG_TYPE::ERR, "Failed to register Gadget.");
  return true;
}

GadgetRemote::GadgetRemote() :
  Remote() {};

GadgetRemote::GadgetRemote(std::shared_ptr<Request_Gadget> gadget) :
  Remote(gadget) {};

void
GadgetRemote::updateCharacteristic(std::string gadget_name, GadgetCharacteristic characteristic, int value) {};