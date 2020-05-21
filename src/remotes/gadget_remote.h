#ifndef __GadgetRemote__
#define __GadgetRemote__

#include "ArduinoJson.h"
#include "../connectors/request_gadget.h"

#include "../system_settings.h"
#include "remote.h"

class GadgetRemote: public Remote {
private:

  virtual bool
  registerGadget(const char *gadget_name, Gadget_Type gadget_type, const char *characteristics) { return false; };

  virtual bool removeGadget(const char *gadget_name) { return false; };

  bool registerGadgetOnRemote(const char *gadget_name, Gadget_Type gadget_type, const char *characteristics) {
    logger.println("Registering Gadget:");
    logger.incIndent();
    if (registerGadget(gadget_name, gadget_type, characteristics)) {
      logger.println(LOG_INFO, "OK");
      logger.decIndent();
      return true;
    } else {
      logger.println(LOG_ERR, "ERR");
      logger.decIndent();
    }
    return false;
  }

  bool handleNewGadget(SH_Gadget * new_gadget) {
    char characteristic_str[HOMEBRIDGE_REGISTER_STR_MAX_LEN - 60];
    new_gadget->getCharacteristics(&characteristic_str[0]);
    if (registerGadgetOnRemote(new_gadget->getName(), new_gadget->getType(), characteristic_str))
      logger.println("Registering Gadget successfull.");
    else
      logger.println(LOG_ERR, "Failed to register Gadget.");
    return true;
  }

public:
  explicit GadgetRemote(Request_Gadget *gadget, JsonObject data) :
    Remote(gadget, data) {};

  virtual void
  updateCharacteristic(const char *gadget_name, const char *service, const char *characteristic, int value) {};

};

#endif //__GadgetRemote__