#include "smarthome_gadget_remote.h"

#include <utility>

bool
SmarthomeGadgetRemote::registerGadget(const std::string& gadget_name, GadgetType gadget_type, vector<GadgetCharacteristic> characteristics) {
  unsigned long ident = micros() % 7023;
  char reg_str[HOMEBRIDGE_REGISTER_STR_MAX_LEN]{};
  const char *service_name;
  if (gadget_type == GadgetType::Lightbulb)
    service_name = "lightbulb";
  else if (gadget_type == GadgetType::Fan)
    service_name = "fan";
  else if (gadget_type == GadgetType::Doorbell)
    service_name = "doorbell";
  else {
    logger.println(LOG_TYPE::ERR, "Unknown Gadget Type");
    return false;
  }
  if (characteristics != nullptr) {
    snprintf(reg_str, HOMEBRIDGE_REGISTER_STR_MAX_LEN,
             R"({"request_id" : %lu, "name": "%s", "service": "%s", "characteristics": {%s}})",
             ident, gadget_name.c_str(), service_name, characteristics);
  } else {
    sprintf(reg_str, R"({"request_id" : %lu, "name": "%s", "service": "%s", "characteristics": {}})", ident,
            gadget_name.c_str(),
            service_name);
  }
//  req_gadget->sendRequest(new Request("smarthome/to/gadget/add", &reg_str[0]));  // TODO: fix
//  unsigned long start_time = millis();
//  while (start_time + 5000 > millis()) {
//    if (!req_gadget->hasRequest()) {
//      req_gadget->refresh();
//    } else {
//      Request *resp = req_gadget->getRequest();
////      if (resp->getPath() == "smarthome/from/response" && getIdent(resp->getPayload()) == ident) {
//      if (resp->getPath() == "smarthome/from/response") {  // TODO: fix
//          delete resp;
//          return getAck(resp->getPayload());
//        }
//      }
//      delete resp;
//    }
//  }
  return false;
}

bool SmarthomeGadgetRemote::removeGadget(const std::string& gadget_name) {  // TODO: fix
//  char buf_msg[HOMEBRIDGE_UNREGISTER_STR_MAX_LEN]{};
//  unsigned long ident = micros() % 7023;
//  snprintf(&buf_msg[0], HOMEBRIDGE_UNREGISTER_STR_MAX_LEN, R"({"request_id" : %lu, "name": "%s"})", ident,
//           gadget_name);
//  req_gadget->sendRequest(new Request("smarthome/to/gadget/remove", &buf_msg[0]));
//  unsigned long start_time = millis();
//  while (start_time + 5000 > millis()) {
//    if (!req_gadget->hasRequest()) {
//      req_gadget->refresh();
//    } else {
//      Request *resp = req_gadget->getRequest();
//      if (resp->getPath() == "smarthome/from/response" && getIdent(resp->getPayload()) == ident) {
//        bool buf_ack = getAck(resp->getPayload());
//        delete resp;
//        return buf_ack;
//      }
//      delete resp;
//    }
//  }
  return false;
}

void SmarthomeGadgetRemote::handleRequest(std::shared_ptr<Request> req) {
  if (req->getPath() == "smarthome/from/set") {
    auto req_body = req->getPayload();
    if (req_body.containsKey("name") && req_body.containsKey("characteristic") && req_body.containsKey("value")) {
      logger.print("System / Gadget-Remote", "Received characteristic update");
      auto target_gadget = gadgets.getGadget(req_body["name"]);
      if (target_gadget != nullptr) {
        auto characteristic = getCharacteristicFromInt(req_body["characteristic"].as<int>());
        if (characteristic != GadgetCharacteristic::None) {
          logger.incIndent();
          lockUpdates();
          int value = req["value"].as<int>();
          target_gadget->handleCharacteristicUpdate(characteristic, value);
          unlockUpdates();
          logger.decIndent();
        } else {
          logger.print(LOG_TYPE::ERR, "Illegal err_characteristic 0");
        }
      } else {
        logger.print("Unknown Gadget");
      }
    }
  } else {
    logger.println("System / Homebridge-Remote", "Received uncomplete Request");
  }
}

SmarthomeGadgetRemote::SmarthomeGadgetRemote(std::shared_ptr<Request_Gadget> gadget) :
  GadgetRemote(std::move(gadget)) {};

void
SmarthomeGadgetRemote::updateCharacteristic(std::string gadget_name, GadgetCharacteristic characteristic, int value) {
  if (updatesAreLocked()) return;
  auto target_gadget = gadgets.getGadget(gadget_name);
  if (characteristic != nullptr && target_gadget != nullptr) {
    char update_str[HOMEBRIDGE_UPDATE_STR_LEN_MAX]{};
    sprintf(&update_str[0],
            R"({"name":"%s","service":"%s","characteristic":"%s","value":%d})",
            gadget_name,
            service,
            characteristic,
            value);
//    req_gadget->sendRequest(new Request("smarthome/to/gadget/update", update_str));  // TODO: fix
  }
}
