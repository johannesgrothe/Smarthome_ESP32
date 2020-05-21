#ifndef __Remote__
#define __Remote__

#include "ArduinoJson.h"
#include "../connectors/request_gadget.h"

#include "../system_settings.h"
#include "../gadgets/sh_gadget.h"
#include "../gadget_collection.h"

class Remote {
private:

  bool lock_updates;
  bool network_initialized;


protected:

  Request_Gadget *req_gadget;
  Gadget_Collection gadgets;

  bool updatesAreLocked() const { return lock_updates; }

  void lockUpdates() {
    logger.println("Locking Updates");
    lock_updates = true;
  }

  void unlockUpdates() {
    logger.println("Unlocking Updates");
    lock_updates = false;
  }

  virtual void handleRequest(const char *path, const char *body) = 0;

  virtual void handleRequest(const char *path, JsonObject body) = 0;

  virtual bool handleNewGadget(SH_Gadget *new_gadget) = 0;

  bool isNetworkInitialised() {
    return network_initialized;
  }

public:
  explicit Remote(JsonObject data) :
    lock_updates(false),
    network_initialized(false),
    req_gadget(nullptr),
    gadgets(){};

  explicit Remote(Request_Gadget *gadget, JsonObject data) :
      lock_updates(false),
      network_initialized(true),
      req_gadget(gadget),
      gadgets(){};

  void handleRequest(Request *req) {
    DynamicJsonDocument body_json(2048);
    DeserializationError err = deserializeJson(body_json, req->getBody());
    if (err == DeserializationError::Ok) {
      JsonObject body = body_json.as<JsonObject>();
      handleRequest(req->getPath(), body);
    } else {
      handleRequest(req->getPath(), req->getBody());
    }
  };

  void addGadget(SH_Gadget *new_gadget) {
    if (gadgets.addGadget(new_gadget)) {
      logger.print(LOG_DATA, "Adding '");
      logger.add(new_gadget->getName());
      logger.addln("'");

      logger.incIndent();
      handleNewGadget(new_gadget);
      logger.decIndent();
    } else {
      logger.print(LOG_ERR, "Unable to add '");
      logger.add(new_gadget->getName());
      logger.addln("'");
    }
  }
};

#endif //__Remote__