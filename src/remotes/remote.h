#ifndef __Remote__
#define __Remote__

#include "ArduinoJson.h"
#include "../connectors/request_gadget.h"

#include "../system_settings.h"
#include "../gadgets/sh_gadget.h"
#include "../gadget_collection.h"

class Remote {
private:

  virtual bool
  registerGadget(const char *gadget_name, Gadget_Type gadget_type, const char *characteristics) { return false; };

  virtual bool removeGadget(const char *gadget_name) { return false; };

  bool lock_updates;

protected:

  Gadget_Collection gadgets;

  bool updatesAreLocked() { return lock_updates; }

  void lockUpdates() {
    logger.println("Locking Updates");
    lock_updates = true;
  }

  void unlockUpdates() {
    logger.println("Unlocking Updates");
    lock_updates = false;
  }

public:
  Remote() :
    lock_updates(false) {
    gadgets = Gadget_Collection();
  };

  virtual void
  updateCharacteristic(const char *gadget_name, const char *service, const char *characteristic, int value) {};

  virtual void handleRequest(const char *path, REQUEST_TYPE type, const char *body) {};

  virtual void handleRequest(const char *path, REQUEST_TYPE type, JsonObject body) {};

  void addGadget(SH_Gadget *new_gadget) {
    if (gadgets.addGadget(new_gadget)) {
      logger.print(LOG_DATA, "Adding '");
      logger.add(new_gadget->getName());
      logger.addln("'");

      logger.incIndent();
      char characteristic_str[HOMEBRIDGE_REGISTER_STR_MAX_LEN - 60];
      new_gadget->getCharacteristics(&characteristic_str[0]);
      if (registerGadget(new_gadget->getName(), new_gadget->getType(), characteristic_str))
        logger.println("Registering Gadget successfull.");
      else
        logger.println(LOG_ERR, "Failed to register Gadget.");
      logger.decIndent();
    } else {
      logger.print(LOG_ERR, "Unable to add '");
      logger.add(new_gadget->getName());
      logger.addln("'");
    }
  }

};

#endif //__Remote__