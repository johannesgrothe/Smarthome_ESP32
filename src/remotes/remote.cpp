#include "remote.h"

#include <utility>

bool Remote::updatesAreLocked() const { return lock_updates; }

void Remote::lockUpdates() {
  logger.println("Locking Updates");
  lock_updates = true;
}

void Remote::unlockUpdates() {
  logger.println("Unlocking Updates");
  lock_updates = false;
}

bool Remote::isNetworkInitialised() const {
  return network_initialized;
}

Remote::Remote() :
  lock_updates(false),
  network_initialized(false),
  req_gadget(nullptr),
  gadgets() {};

Remote::Remote(std::shared_ptr<Request_Gadget> gadget, std::string chip_name) :
  lock_updates(false),
  network_initialized(true),
  chip_name(std::move(chip_name)),
  req_gadget(std::move(gadget)),
  gadgets() {};

void Remote::addGadget(const std::shared_ptr<SH_Gadget>& new_gadget) {
  if (gadgets.addGadget(new_gadget)) {
    logger.print(LOG_TYPE::DATA, "Adding '%s'", new_gadget->getName());

    logger.incIndent();
    handleNewGadget(new_gadget);
    logger.decIndent();
  } else {
    logger.print(LOG_TYPE::ERR, "Unable to add '%s'", new_gadget->getName());
  }
}

void Remote::handleRequest(Request *req) {
 // TODO
}
