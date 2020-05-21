#ifndef __Smarthome_Remote__
#define __Smarthome_Remote__

#include "../connectors/mqtt_gadget.h"
#include "gadget_remote.h"


class SmarthomeRemote: public GadgetRemote {
private:
  MQTT_Gadget *mqtt_gadget;

  bool registerGadget(const char *gadget_name, Gadget_Type gadget_type, const char *characteristics) override {
    unsigned long ident = micros() % 7023;
    char reg_str[HOMEBRIDGE_REGISTER_STR_MAX_LEN]{};
    const char *service_name;
    if (gadget_type == Lightbulb)
      service_name = "lightbulb";
    else if (gadget_type == Fan)
      service_name = "fan";
    else if (gadget_type == Doorbell)
      service_name = "doorbell";
    else {
      logger.println(LOG_ERR, "Unknown Gadget Type");
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
    mqtt_gadget->sendRequest("smarthome/to/gadget/add", &reg_str[0]);
    unsigned long start_time = millis();
    while (start_time + 5000 > millis()) {
      if (!mqtt_gadget->hasRequest()) {
        mqtt_gadget->refresh();
      } else {
        Request *resp = mqtt_gadget->getRequest();
        if (strcmp(resp->getPath(), "smarthome/from/response") == 0 && getIdent(resp->getBody()) == ident) {
          delete resp;
          return getAck(resp->getBody());
        }
        delete resp;
      }
    }
    return false;
  };

  bool removeGadget(const char *gadget_name) override {
    char buf_msg[HOMEBRIDGE_UNREGISTER_STR_MAX_LEN]{};
    unsigned long ident = micros() % 7023;
    snprintf(&buf_msg[0], HOMEBRIDGE_UNREGISTER_STR_MAX_LEN, R"({"request_id" : %lu, "name": "%s"})", ident,
             gadget_name);
    mqtt_gadget->sendRequest("smarthome/to/gadget/remove", &buf_msg[0]);
    unsigned long start_time = millis();
    while (start_time + 5000 > millis()) {
      if (!mqtt_gadget->hasRequest()) {
        mqtt_gadget->refresh();
      } else {
        Request *resp = mqtt_gadget->getRequest();
        if (strcmp(resp->getPath(), "smarthome/from/response") == 0 && getIdent(resp->getBody()) == ident) {
          delete resp;
          return getAck(resp->getBody());
        }
        delete resp;
      }
    }
    return false;
  };

public:
  SmarthomeRemote(MQTT_Gadget *new_mqtt_gadget, JsonObject data) :
    GadgetRemote(data),
    mqtt_gadget(new_mqtt_gadget) {};

  void
  updateCharacteristic(const char *gadget_name, const char *service, const char *characteristic, int value) override {
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
      mqtt_gadget->sendRequest("smarthome/to/gadget/update", update_str);
    }
  };

  void handleRequest(REQUEST_TYPE type, const char *path, const char *body) override {
    logger.println(LOG_ERR, "Smarthome-Remote cannot handle String Bodys.");
  };

  void handleRequest(REQUEST_TYPE type, const char *path, JsonObject body) override {
    if (type == REQ_MQTT && strcmp(path, "smarthome/from/set") == 0) {
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
};

#endif //__Smarthome_Remote__