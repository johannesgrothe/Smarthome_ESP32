#include "remote.h"

bool Remote::updatesAreLocked() const { return lock_updates; }

void Remote::lockUpdates() {
  logger.println("Locking Updates");
  lock_updates = true;
}

void Remote::unlockUpdates() {
  logger.println("Unlocking Updates");
  lock_updates = false;
}

bool Remote::isNetworkInitialised() {
  return network_initialized;
}

Remote::Remote(JsonObject data) :
  lock_updates(false),
  network_initialized(false),
  req_gadget(nullptr),
  gadgets() {};

Remote::Remote(Request_Gadget *gadget, JsonObject data) :
  lock_updates(false),
  network_initialized(true),
  req_gadget(gadget),
  gadgets() {};

void Remote::handleRequest(Request *req) {
  DynamicJsonDocument body_json(2048);
  DeserializationError err = deserializeJson(body_json, req->getBody());
  if (err == DeserializationError::Ok) {
    JsonObject body = body_json.as<JsonObject>();
    handleRequest(req->getPath(), body);
  } else {
    handleRequest(req->getPath(), req->getBody());
  }
}

void Remote::addGadget(SH_Gadget *new_gadget) {
  if (gadgets.addGadget(new_gadget)) {
    logger.print(LOG_TYPE::DATA, "Adding '");
    logger.add(new_gadget->getName());
    logger.addln("'");

    logger.incIndent();
    handleNewGadget(new_gadget);
    logger.decIndent();
  } else {
    logger.print(LOG_TYPE::ERR, "Unable to add '");
    logger.add(new_gadget->getName());
    logger.addln("'");
  }
}