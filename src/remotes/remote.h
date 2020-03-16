#ifndef __Remote__
#define __Remote__

#include "ArduinoJson.h"
#include "../connectors/request_gadget.h"

#include "../system_settings.h"
#include "../gadgets/sh_gadget.h"

class Remote {
private:

  SH_Gadget *gadgets[MAIN_MAX_GADGETS]{};

  byte gadget_count;


  virtual bool
  registerGadget(const char *gadget_name, Gadget_Type gadget_type, const char *characteristics) { return false; };

  virtual bool removeGadget(const char *gadget_name) { return false; };

  bool lock_updates;

protected:

  bool updatesAreLocked() { return lock_updates; }

  void lockUpdates() {
    logger.println("Locking Updates");
    lock_updates = true;
  }

  void unlockUpdates() {
    logger.println("Unlocking Updates");
    lock_updates = false;
  }

  SH_Gadget *getGadgetForName(const char *name) {
    for (byte k = 0; k < gadget_count; k++) {
      SH_Gadget *it_gadget = gadgets[k];
      if (strcmp(it_gadget->getName(), name) == 0) {
        return it_gadget;
      }
    }
    return nullptr;
  }


public:
  Remote() :
    gadget_count(0),
    lock_updates(false) {};

  virtual void
  updateCharacteristic(const char *gadget_name, const char *service, const char *characteristic, int value) {};

  virtual void
  updateCharacteristic(const char *gadget_name, const char *service, const char *characteristic, bool value) {};

  virtual void handleRequest(const char *path, REQUEST_TYPE type, const char *body) {};

  virtual void handleRequest(const char *path, REQUEST_TYPE type, JsonObject body) {};

  void addGadget(SH_Gadget *new_gadget) {
    if (gadget_count < (MAIN_MAX_GADGETS - 1)) {
      gadgets[gadget_count] = new_gadget;
      gadget_count++;

      char characteristic_str[HOMEBRIDGE_REGISTER_STR_MAX_LEN - 60];
      new_gadget->getCharacteristics(&characteristic_str[0]);
      registerGadget(new_gadget->getName(), new_gadget->getType(), characteristic_str);
    }
  }

};

#endif //__Remote__