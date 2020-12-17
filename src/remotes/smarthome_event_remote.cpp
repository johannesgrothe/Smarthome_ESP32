#include "smarthome_event_remote.h"

bool SmarthomeEventRemote::registerGadget(const std::string &gadget_name, GadgetType gadget_type,
                                          vector<GadgetCharacteristicSettings> characteristics) {}

bool SmarthomeEventRemote::removeGadget(const std::string &gadget_name) {}

void SmarthomeEventRemote::handleRequest(std::shared_ptr<Request> req) {
  if (req->getPath() == "smarthome/remotes/event/receive") {
    auto req_body = req->getPayload();
    if (req_body.containsKey("name") && req_body.containsKey("timestamp") && req_body.containsKey("event_type")) {
      logger.print("System / Event-Remote", "Received event_type update");
      logger.incIndent();
      auto sender = req_body["name"].as<string>();
      auto timestamp = req_body["timestamp"].as<unsigned long long>();
      auto type = EventType(req_body["event_type"].as<int>());
      auto event_buf = std::make_shared<Event>(sender, timestamp, type);
      forwardEvent(event_buf);
      logger.decIndent();
    } else {
      logger.print("Unknown Event");
    }
  }
}

void SmarthomeEventRemote::sendEvent(string sender, EventType type) {
  if (updatesAreLocked()) return;
  if (sender.empty()) {
    logger.println("no sender specified, no event send");
    return;
  }
  unsigned long long timestamp = millis();
  auto event_buf = std::make_shared<Event>(sender, timestamp, type);

  DynamicJsonDocument req_doc(2000);

  req_doc["name"] = sender;
  req_doc["timestamp"] = timestamp;
  req_doc["event_type"] = int(type);

  auto out_req = std::make_shared<Request>("smarthome/remotes/event/send", timestamp, chip_name, "<remote>", req_doc);

  req_gadget->sendRequest(out_req);

  forwardEvent(event_buf);
}

SmarthomeEventRemote::SmarthomeEventRemote(std::shared_ptr<Request_Gadget> gadget) : EventRemote(gadget) {

};

