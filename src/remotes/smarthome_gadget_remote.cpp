#include "smarthome_gadget_remote.h"

bool
SmarthomeGadgetRemote::registerGadget(const char *gadget_name, GadgetType gadget_type, const char *characteristics) {
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
             ident, gadget_name, service_name, characteristics);
  } else {
    sprintf(reg_str, R"({"request_id" : %lu, "name": "%s", "service": "%s", "characteristics": {}})", ident,
            gadget_name,
            service_name);
  }
  req_gadget->sendRequest(new Request("smarthome/to/gadget/add", &reg_str[0]));
  unsigned long start_time = millis();
  while (start_time + 5000 > millis()) {
    if (!req_gadget->hasRequest()) {
      req_gadget->refresh();
    } else {
      Request *resp = req_gadget->getRequest();
      if (strcmp(resp->getPath(), "smarthome/from/response") == 0 && getIdent(resp->getBody()) == ident) {
        delete resp;
        return getAck(resp->getBody());
      }
      delete resp;
    }
  }
  return false;
}

bool SmarthomeGadgetRemote::removeGadget(const char *gadget_name) {
  char buf_msg[HOMEBRIDGE_UNREGISTER_STR_MAX_LEN]{};
  unsigned long ident = micros() % 7023;
  snprintf(&buf_msg[0], HOMEBRIDGE_UNREGISTER_STR_MAX_LEN, R"({"request_id" : %lu, "name": "%s"})", ident,
           gadget_name);
  req_gadget->sendRequest(new Request("smarthome/to/gadget/remove", &buf_msg[0]));
  unsigned long start_time = millis();
  while (start_time + 5000 > millis()) {
    if (!req_gadget->hasRequest()) {
      req_gadget->refresh();
    } else {
      Request *resp = req_gadget->getRequest();
      if (strcmp(resp->getPath(), "smarthome/from/response") == 0 && getIdent(resp->getBody()) == ident) {
        delete resp;
        return getAck(resp->getBody());
      }
      delete resp;
    }
  }
  return false;
}

void SmarthomeGadgetRemote::handleRequest(const char *path, const char *body) {
  logger.println(LOG_TYPE::ERR, "Smarthome-Remote cannot handle String Bodys.");
}

void SmarthomeGadgetRemote::handleRequest(const char *path, const JsonObject& body) {
  if (strcmp(path, "smarthome/from/set") == 0) {
    if (body["name"] != nullptr && body["characteristic"] != nullptr && body["value"] != nullptr) {
      logger.print("System / Gadget-Remote", "Received valid Request: ");
      SH_Gadget *target_gadget = gadgets.getGadget(body["name"].as<const char *>());
      if (target_gadget != nullptr) {
        const char *characteristic = body["characteristic"].as<const char *>();
        logger.add(target_gadget->getName());
        logger.add("/");
        logger.addln(characteristic);
        logger.incIndent();
        lockUpdates();
        int value = body["value"].as<int>();
        target_gadget->handleCharacteristicUpdate(characteristic, value);
        unlockUpdates();
        logger.decIndent();
      } else {
        logger.add("Unknown Gadget");
      }
    }
  } else {
    logger.println("System / Homebridge-Remote", "Received uncomplete Request");
  }
}

SmarthomeGadgetRemote::SmarthomeGadgetRemote(Request_Gadget *gadget, JsonObject data) :
  GadgetRemote(gadget, data) {};

void
SmarthomeGadgetRemote::updateCharacteristic(const char *gadget_name, const char *service, const char *characteristic,
                                            int value) {
  if (updatesAreLocked()) return;
  SH_Gadget *target_gadget = gadgets.getGadget(gadget_name);
  if (characteristic != nullptr && target_gadget != nullptr) {
    char update_str[HOMEBRIDGE_UPDATE_STR_LEN_MAX]{};
    sprintf(&update_str[0],
            R"({"name":"%s","service":"%s","characteristic":"%s","value":%d})",
            gadget_name,
            service,
            characteristic,
            value);
    req_gadget->sendRequest(new Request("smarthome/to/gadget/update", update_str));
  }
}