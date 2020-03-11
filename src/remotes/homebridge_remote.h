#ifndef __Homebridge_Remote__
#define __Homebridge_Remote__

#include "../connectors/mqtt_gadget.h"
#include "remote.h"

class Homebridge_Remote : public Remote {
private:

  MQTT_Gadget *mqtt_gadget;

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

  bool registerGadget(const char *gadget_name, const char *service, const char *characteristics) override {
    char reg_str[HOMEBRIDGE_REGISTER_STR_MAX_LEN]{};
    char characteristic_buffer[HOMEBRIDGE_REGISTER_STR_MAX_LEN - 80]{};
    if (characteristics != nullptr) {
      snprintf(reg_str, HOMEBRIDGE_REGISTER_STR_MAX_LEN, R"({"name": "%s", "service_name": "%s", "service": "%s", %s})",
               gadget_name, gadget_name, service, characteristic_buffer);
    } else {
      sprintf(reg_str, R"({"name": "%s", "service_name": "%s", "service": "%s"})", gadget_name, gadget_name, service);
    }
    return mqtt_gadget->publishMessageAndWaitForAnswer("homebridge/to/add", &reg_str[0]);
  };

  bool removeGadget(const char *gadget_name) override {
    char buf_msg[HOMEBRIDGE_UNREGISTER_STR_MAX_LEN]{};
    snprintf(&buf_msg[0], HOMEBRIDGE_UNREGISTER_STR_MAX_LEN, R"({"name": "%s"})", gadget_name);
    return mqtt_gadget->publishMessageAndWaitForAnswer("homebridge/to/remove", &buf_msg[0]);
  };

};

#endif //__Homebridge_Remote__