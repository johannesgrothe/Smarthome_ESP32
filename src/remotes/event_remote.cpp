#include "event_remote.h"

void EventRemote::handleRequest(std::string path, std::string body) {
  logger.println(LOG_TYPE::ERR, "Event-Remote is rekt.");
}

void EventRemote::handleRequest(std::string path, const JsonObject& body) {
  if (path == "smarthome/remotes/event/receive") {
    if (body["name"] != nullptr && body["characteristic"] != nullptr && body["value"] != nullptr) {
      logger.print("System / Gadget-Remote", "Received valid Request: ");
      auto target_gadget = gadgets.getGadget(body["name"].as<const char *>());
      if (target_gadget != nullptr) {
        const char *characteristic = body["characteristic"].as<const char *>();
        logger.print(target_gadget->getName());
        logger.print("/");
        logger.println(characteristic);
        logger.incIndent();
        lockUpdates();
        int value = body["value"].as<int>();
        target_gadget->handleCharacteristicUpdate(characteristic, value);
        unlockUpdates();
        logger.decIndent();
      } else {
        logger.print("Unknown Gadget");
      }
    }
  } else {
    logger.println("System / Homebridge-Remote", "Received uncomplete Request");
  }
}

EventRemote::EventRemote() :
    Remote() {};

EventRemote::EventRemote(std::shared_ptr<Request_Gadget> gadget) :
    Remote(gadget) {};


