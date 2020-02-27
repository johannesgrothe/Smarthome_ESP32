#ifndef __Homebridge_Connector__
#define __Homebridge_Connector__

#include "../system_settings.h"
#include "mqtt_gadget.h"
#include <cstring>

// Connector for MQTT Usage
class Homebridge_Connector {
protected:

  const char *mqtt_gadget_name;

  char mqtt_service_type[HOMEBRIDGE_SERVICE_TYPE_LEN_MAX]{};

  MQTT_Gadget *homebridge_mqtt_gadget;

  bool initialized_homebridge;

  bool registerHomebridgeGadget() {
    char reg_str[HOMEBRIDGE_REGISTER_STR_MAX_LEN]{};
    if (getHomebridgeRegisterStr(&reg_str[0])) {
      return homebridge_mqtt_gadget->publishMessage("homebridge/to/add", &reg_str[0]);
    }
    return false;
  }

  bool unregisterHomebridgeGadget() {
    char buf_msg[HOMEBRIDGE_UNREGISTER_STR_MAX_LEN]{};
    if (getHomebridgeUnregisterStr(&buf_msg[0])) {
      return homebridge_mqtt_gadget->publishMessage("homebridge/to/remove", &buf_msg[0]);
    }
    return false;
  }

  virtual bool getHomebridgeRegisterStr(char *buffer) {}

  bool getHomebridgeUnregisterStr(char *buffer) {
    snprintf(&buffer[0], HOMEBRIDGE_UNREGISTER_STR_MAX_LEN, R"({"name": "%s"})", mqtt_gadget_name);
    return true;
  }

  void updateHomebridge(JsonObject data) {
//    TODO: update_str is empty on initialization and null after serialization
//    char update_str[HOMEBRIDGE_UPDATE_STR_LEN_MAX]{};
//    Serial.println(update_str);
//    serializeJson(data, &update_str[0], HOMEBRIDGE_UPDATE_STR_LEN_MAX);
//    Serial.println(update_str);
//    updateHomebridge(&update_str[0]);
  }

  void updateHomebridge(const char *data) {
    homebridge_mqtt_gadget->publishMessage("homebridge/to/set", data);
  }

  void decodeHomebridgeCommand(JsonObject data) {
    if (data["name"] != nullptr && data["characteristic"] != nullptr && data["value"] != nullptr) {
      logger.println(data["name"].as<const char *>());
      logger.println(mqtt_gadget_name);
      if (strcmp(mqtt_gadget_name, data["name"].as<const char *>()) == 0) {
        logger.println("Correct Name");
        int value;
        const char *characteristc = data["characteristic"].as<const char *>();
        if (data["value"] == "true")
          value = 1;
        else if (data["value"] == "false")
          value = 0;
        else
          value = data["value"].as<int>();
        applyHomebridgeCommand(characteristc, value);
      }
    }
  }

  void setHomebridgeServiceType(const char *service_type) {
    strncpy(&mqtt_service_type[0], service_type, HOMEBRIDGE_SERVICE_TYPE_LEN_MAX);
  }

public:

  Homebridge_Connector() :
    mqtt_gadget_name(nullptr),
    homebridge_mqtt_gadget(nullptr),
    initialized_homebridge(false) {
    Serial.println(mqtt_service_type[0] == '\n');
  };

  void updateHomebridgeCharacteristic(const char *characteristic, int value) {
    if (characteristic != nullptr) {
      char update_str[HOMEBRIDGE_UPDATE_STR_LEN_MAX]{};
      sprintf(&update_str[0],
              "{\"name\":\"%s\",\"service_name\":\"%s\",\"service_type\":\"%s\",\"characteristic\":\"%s\",\"value\":%d}",
              mqtt_gadget_name,
              mqtt_gadget_name,
              &mqtt_service_type[0],
              characteristic,
              value);
      updateHomebridge(&update_str[0]);
    }
  }

  void updateHomebridgeCharacteristic(const char *characteristic, bool value) {
    if (characteristic != nullptr) {
      char bool_str[6]{};
      if (value)
        strcpy(bool_str, "true");
      else
        strcpy(bool_str, "false");
      char update_str[HOMEBRIDGE_UPDATE_STR_LEN_MAX]{};
      sprintf(&update_str[0],
              "{\"name\":\"%s\",\"service_name\":\"%s\",\"service_type\":\"%s\",\"characteristic\":\"%s\",\"value\":%s}",
              mqtt_gadget_name,
              mqtt_gadget_name,
              &mqtt_service_type[0],
              characteristic,
              &bool_str[0]);
      updateHomebridge(&update_str[0]);
    }

//    if (characteristic != nullptr) {
//      try {
//        DynamicJsonDocument json_file(2048);
//        JsonObject json_doc = json_file.as<JsonObject>();
//        json_doc["name"] = mqtt_gadget_name;
//        json_doc["characteristic"] = characteristic;
//        if (value) {
//          json_doc["value"] = "True";
//        } else {
//          json_doc["value"] = "False";
//        }
//        updateHomebridge(json_doc);
//      }
//      catch (DeserializationError &e) {
//        logger.println(LOG_ERR, "Invalid Json in 'updateHomebridgeCharacteristic'");
//      }
//    }
  }

  void initHomebridgeConnector(MQTT_Gadget *new_mqtt_gadget, const char *gadget_name) {
    if (new_mqtt_gadget == nullptr || gadget_name == nullptr || strcpy(&mqtt_service_type[0], "") == 0)
      return;
    mqtt_gadget_name = gadget_name;
    homebridge_mqtt_gadget = new_mqtt_gadget;
    logger.println(LOG_DATA, "Homebridge");
    logger.incIntent();
    logger.print(LOG_DATA, "Name: ");
    logger.addln(mqtt_gadget_name);
    delay(15);
    if (unregisterHomebridgeGadget()) {
      logger.println(LOG_INFO, "Unregistered previous Gadget");
    } else {
      logger.println(LOG_ERR, "Could not Unregister previous Gadget");
    }
    delay(25);
    if (registerHomebridgeGadget()) {
      logger.println(LOG_INFO, "Registered new Gadget");
    } else {
      logger.println(LOG_ERR, "Could not register new Gadget on Server");
    }
    initialized_homebridge = true;
    logger.decIntent();
  }

  bool initializedHomebridge() {
    return initialized_homebridge;
  }

  virtual void applyHomebridgeCommand(const char *characteristic, int value) {};

};

#endif //__Homebridge_Connector__