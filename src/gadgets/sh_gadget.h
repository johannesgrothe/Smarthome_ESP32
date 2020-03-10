#include <cstring>
#include <Arduino.h>
#include "ArduinoJson.h"
#include "../colors.h"
#include "../helping_structures.h"
#include "../system_settings.h"
#include "../console_logger.h"
#include "../connectors/connectors.h"

#ifndef __SH_Gadget__
#define __SH_Gadget__

enum SH_RGB_Color {
  SH_CLR_red, SH_CLR_green, SH_CLR_blue
};
enum SH_HSL_Color {
  SH_CLR_hue, SH_CLR_saturation, SH_CLR_lightness
};

class SH_Gadget : public IR_Connector, public Radio_Connector {
private:


protected:
  // Main Gadget
  char name[GADGET_NAME_LEN_MAX]{};
  bool initialized;
  bool has_changed;
  byte mapping_count{};
  Mapping_Reference *mapping[MAPPING_MAX_COMMANDS]{};
  // End Main Gadget

  // Basic Code Connector
  const char *findMethodForCode(unsigned long code) {
    for (byte k = 0; k < mapping_count; k++) {
      if (mapping[k]->containsCode(code)) {
        const char *method_name = mapping[k]->getName();
        logger.printname(name, "-> ");
        logger.addln(method_name);
        return method_name;
      }
    }
    return nullptr;
  }

  virtual void applyMappingMethod(const char *method) {}
  // End Basic Code Connector

  // Homebridge Connector
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
  // End Homebridge Connector

public:
  SH_Gadget() :
    name("default"),
    initialized(false),
    has_changed(true) {};

  explicit SH_Gadget(JsonObject gadget) :
    initialized(false),
    has_changed(true) {
    if (gadget["name"] != nullptr) {
      byte namelen =
        strlen(gadget["name"].as<const char *>()) < GADGET_NAME_LEN_MAX ? strlen(gadget["name"].as<const char *>())
                                                                        : GADGET_NAME_LEN_MAX;
      strncpy(name, gadget["name"].as<const char *>(), namelen);
    } else {
      strcpy(name, "Unknown");
      logger.println(LOG_ERR, "No Name found!");
    }
    if (gadget["mapping"] != nullptr) {
      JsonObject local_mapping = gadget["mapping"].as<JsonObject>();
      mapping_count = local_mapping.size() < MAPPING_MAX_COMMANDS ? local_mapping.size() : MAPPING_MAX_COMMANDS;
      logger.print(LOG_INFO, "Configuring Mapping, Commands: ");
      logger.addln(mapping_count);
      logger.incIntent();
      byte j = 0;
      for (auto &&com : local_mapping) {
        if (j < mapping_count) {
          const char *new_name = com.key().c_str();
          JsonArray buf_arr = com.value().as<JsonArray>();
          mapping[j] = new Mapping_Reference(buf_arr, new_name);
          j++;
        }
      }
      logger.decIntent();
      logger.println("Method Mapping loaded.");
    } else {
      logger.println(LOG_WARN, "No Mapping Found.");
    }
  };

  void initConnectors(MQTT_Gadget *mqtt_gadget) {
    initHomebridgeConnector(mqtt_gadget);
  }

  const char *getName() {
    return &name[0];
  }

  bool isInitialized() {
    return initialized;
  };

  virtual bool init() {
    initialized = true;
    return false;
  };

  void handleCode(unsigned long code) {
    const char *method_name = findMethodForCode(code);
    logger.incIntent();
    applyMappingMethod(method_name);
    logger.decIntent();
  }

  void handleRequest(REQUEST_TYPE type, const char *path, const char *body) {
//    logger.println("Decoding String");
  }

  void handleRequest(REQUEST_TYPE type, const char *path, JsonObject body) {
    if (type == REQ_MQTT && strcmp(path, "homebridge/from/set") == 0) {
      decodeHomebridgeCommand(body);
    }
  }

  virtual void refresh() {}

  virtual void print() {}

  void printMapping() {
    logger.printname(name, "Accessible Methods: ");
    logger.addln(mapping_count);
    logger.incIntent();
    for (byte k = 0; k < mapping_count; k++) {
      mapping[k]->printMapping();
    }
    logger.decIntent();
  }

};

#endif //__SH_GAGDET__