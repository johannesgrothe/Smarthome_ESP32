#ifndef __Homebridge_Remote__
#define __Homebridge_Remote__

#include "../connectors/mqtt_gadget.h"
#include "remote.h"

class Homebridge_Remote : public Remote {
private:

  MQTT_Gadget *mqtt_gadget;

  bool registerGadget(const char *gadget_name, Gadget_Type gadget_type, const char *characteristics) override {
    removeGadget(gadget_name);
    char reg_str[HOMEBRIDGE_REGISTER_STR_MAX_LEN]{};
    const char *service_name;
    if (gadget_type == Lightbulb)
      service_name = "Lightbulb";
    else if (gadget_type == Fan)
      service_name = "Fan";
    else if (gadget_type == Doorbell)
      service_name = "Doorbell";
    else {
      logger.println(LOG_ERR, "Unknown Gadget Type");
      return false;
    }
    if (characteristics != nullptr) {
      snprintf(reg_str, HOMEBRIDGE_REGISTER_STR_MAX_LEN, R"({"name": "%s", "service_name": "%s", "service": "%s", %s})",
               gadget_name, gadget_name, service_name, characteristics);
    } else {
      sprintf(reg_str, R"({"name": "%s", "service_name": "%s", "service": "%s"})", gadget_name, gadget_name,
              service_name);
    }
    return mqtt_gadget->publishMessageAndWaitForAnswer("homebridge/to/add", &reg_str[0]);
  };

  bool removeGadget(const char *gadget_name) override {
    char buf_msg[HOMEBRIDGE_UNREGISTER_STR_MAX_LEN]{};
    snprintf(&buf_msg[0], HOMEBRIDGE_UNREGISTER_STR_MAX_LEN, R"({"name": "%s"})", gadget_name);
    return mqtt_gadget->publishMessageAndWaitForAnswer("homebridge/to/remove", &buf_msg[0]);
  };

public:
  Homebridge_Remote(MQTT_Gadget *new_mqtt_gadget) :
    Remote(),
    mqtt_gadget(new_mqtt_gadget) {};

  void
  updateCharacteristic(const char *gadget_name, const char *service, const char *characteristic, int value) override {
    if (updatesAreLocked()) return;
    SH_Gadget *target_gadget = gadgets.getGadget(gadget_name);
    if (characteristic != nullptr && target_gadget != nullptr) {
      char update_str[HOMEBRIDGE_UPDATE_STR_LEN_MAX]{};
      sprintf(&update_str[0],
              "{\"name\":\"%s\",\"service_name\":\"%s\",\"service_type\":\"%s\",\"characteristic\":\"%s\",\"value\":%d}",
              gadget_name,
              gadget_name,
              service,
              characteristic,
              value);
      mqtt_gadget->publishMessage("homebridge/to/set", update_str);
    }
  };

  void handleRequest(const char *path, REQUEST_TYPE type, const char *body) override {
    logger.println(LOG_ERR, "Homebridge-Remote cannot handle String Bodys.");
  };

  void handleRequest(const char *path, REQUEST_TYPE type, JsonObject body) override {
    if (type == REQ_MQTT && strcmp(path, "homebridge/from/set") == 0) {
      if (body["name"] != nullptr && body["characteristic"] != nullptr && body["value"] != nullptr) {
        logger.print("System / Homebridge-Remote", "Received valid Request: ");
        SH_Gadget *target_gadget = gadgets.getGadget(body["name"].as<const char *>());
        if (target_gadget != nullptr) {
          const char *characteristc = body["characteristic"].as<const char *>();
          logger.add(target_gadget->getName());
          logger.add("/");
          logger.addln(characteristc);
          logger.incIndent();
          lockUpdates();
          int value = body["value"].as<int>();
          target_gadget->handleCharacteristicUpdate(characteristc, value);
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

#endif //__Homebridge_Remote__