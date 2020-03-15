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
    char characteristic_buffer[HOMEBRIDGE_REGISTER_STR_MAX_LEN - 80]{};
    const char *service_name = "Lightbulb";
    if (characteristics != nullptr) {
      snprintf(reg_str, HOMEBRIDGE_REGISTER_STR_MAX_LEN, R"({"name": "%s", "service_name": "%s", "service": "%s", %s})",
               gadget_name, gadget_name, service_name, characteristic_buffer);
    } else {
      sprintf(reg_str, R"({"name": "%s", "service_name": "%s", "service": "%s"})", gadget_name, gadget_name, service_name);
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

  void updateCharacteristic(const char *gadget_name, const char *service, const char *characteristic, int value) override {

  };

  void updateCharacteristic(const char *gadget_name, const char *service, const char *characteristic, bool value) override {

  };

  void handleRequest(const char *path, REQUEST_TYPE type, const char *body) override {

  };

  void handleRequest(const char *path, REQUEST_TYPE type, JsonObject body) override {

  };

};

#endif //__Homebridge_Remote__

#if false

  char homebridge_service_type[HOMEBRIDGE_SERVICE_TYPE_LEN_MAX]{};
  MQTT_Gadget *homebridge_mqtt_gadget;

  bool registerHomebridgeGadget() {
    char reg_str[HOMEBRIDGE_REGISTER_STR_MAX_LEN]{};
    char characteristic_buffer[HOMEBRIDGE_REGISTER_STR_MAX_LEN - 80]{};
    if (getHomebridgeCharacteristics(&characteristic_buffer[0])) {
      snprintf(reg_str, HOMEBRIDGE_REGISTER_STR_MAX_LEN, R"({"name": "%s", "service_name": "%s", "service": "%s", %s})",
               name, name, homebridge_service_type, characteristic_buffer);
    } else {
      sprintf(reg_str, R"({"name": "%s", "service_name": "%s", "service": "%s"})", name, name, homebridge_service_type);
    }
    return homebridge_mqtt_gadget->publishMessageAndWaitForAnswer("homebridge/to/add", &reg_str[0]);
  }

  bool unregisterHomebridgeGadget() {
    char buf_msg[HOMEBRIDGE_UNREGISTER_STR_MAX_LEN]{};
    snprintf(&buf_msg[0], HOMEBRIDGE_UNREGISTER_STR_MAX_LEN, R"({"name": "%s"})", name);
    return homebridge_mqtt_gadget->publishMessageAndWaitForAnswer("homebridge/to/remove", &buf_msg[0]);
  }

  void initHomebridgeConnector(MQTT_Gadget *new_gadget) {
    logger.println("Homebridge:");
    logger.incIntent();
    setHomebridgeMQTTGadget(new_gadget);
    if (unregisterHomebridgeGadget())
      logger.println(LOG_DATA, "Unregistered old gadget");
    if (registerHomebridgeGadget())
      logger.println(LOG_DATA, "Registered new gadget");
    logger.decIntent();
  }

  void updateHomebridge(const char *data) {
    homebridge_mqtt_gadget->publishMessage("homebridge/to/set", data);
  }

  void decodeHomebridgeCommand(JsonObject data) {
    if (data["name"] != nullptr && data["characteristic"] != nullptr && data["value"] != nullptr) {
      if (strcmp(name, data["name"].as<const char *>()) == 0) {
        int value;
        const char *characteristc = data["characteristic"].as<const char *>();
        logger.printname(name, "-> ");
        logger.addln(characteristc);
        if (data["value"] == "true")
          value = 1;
        else if (data["value"] == "false")
          value = 0;
        else
          value = data["value"].as<int>();
        logger.incIntent();
        applyHomebridgeCommand(characteristc, value);
        logger.decIntent();
      }
    }
  }

  void setHomebridgeServiceType(const char *service_type) {
    strncpy(&homebridge_service_type[0], service_type, HOMEBRIDGE_SERVICE_TYPE_LEN_MAX);
  }

  void setHomebridgeMQTTGadget(MQTT_Gadget *new_gadget) {
    logger.println(LOG_DATA, "Setting MQTT Gadget");
    homebridge_mqtt_gadget = new_gadget;
  }

  void updateHomebridgeCharacteristic(const char *characteristic, int value, bool do_update = true) {
    if (characteristic != nullptr && do_update) {
      char update_str[HOMEBRIDGE_UPDATE_STR_LEN_MAX]{};
      sprintf(&update_str[0],
              "{\"name\":\"%s\",\"service_name\":\"%s\",\"service_type\":\"%s\",\"characteristic\":\"%s\",\"value\":%d}",
              name,
              name,
              &homebridge_service_type[0],
              characteristic,
              value);
      updateHomebridge(&update_str[0]);
    }
  }

  void updateHomebridgeCharacteristic(const char *characteristic, bool value, bool do_update = true) {
    if (characteristic != nullptr && do_update) {
      char bool_str[6]{};
      if (value)
        strcpy(bool_str, "true");
      else
        strcpy(bool_str, "false");
      char update_str[HOMEBRIDGE_UPDATE_STR_LEN_MAX]{};
      sprintf(&update_str[0],
              "{\"name\":\"%s\",\"service_name\":\"%s\",\"service_type\":\"%s\",\"characteristic\":\"%s\",\"value\":%s}",
              name,
              name,
              &homebridge_service_type[0],
              characteristic,
              &bool_str[0]);
      updateHomebridge(&update_str[0]);
    }
  }

  virtual bool getHomebridgeCharacteristics(char *buffer) { return false; }

  virtual void applyHomebridgeCommand(const char *characteristic, int value) {};

#endif