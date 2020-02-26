#include <cstring>
#include <Arduino.h>
#include "../connectors/serial_connector.h"
#include "../connectors/radio_connector.h"
#include "../connectors/ir_connector.h"
#include "../connectors/mqtt_connector.h"
#include "ArduinoJson.h"
#include "../colors.h"
#include "../helping_structures.h"
#include "../system_settings.h"
#include "../console_logger.h"

#ifndef __SH_Gadget__
#define __SH_Gadget__

enum SH_RGB_Color {
  SH_CLR_red, SH_CLR_green, SH_CLR_blue
};
enum SH_HSL_Color {
  SH_CLR_hue, SH_CLR_saturation, SH_CLR_lightness
};

class SH_Gadget : public Homebridge_Connector {
protected:
  char name[GADGET_NAME_LEN_MAX]{};
  bool initialized;
  bool has_changed;

  byte mapping_count{};

  Mapping_Reference *mapping[MAPPING_MAX_COMMANDS]{};

  const char *findMethodForCode(unsigned long code) {
    for (byte k = 0; k < mapping_count; k++) {
      if (mapping[k]->containsCode(code)) {
        return mapping[k]->getName();
      }
    }
    return nullptr;
  }

public:
  SH_Gadget() :
    name("default"),
    initialized(false),
    has_changed(true) {
  };

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
    initHomebridgeConnector(mqtt_gadget, name);
  }

  /**
  * @return The Name of the Gadget
  */
  const char *getName() {
    return &name[0];
  }

  /**
  * @return Whether the Gadget is initialized or not
  */
  bool isInitialized() {
    return initialized;
  };

  virtual bool init() {
    initialized = true;
    return false;
  };

  void decodeRequest(REQUEST_TYPE type, const char *path, const char *body) {
    logger.println("Decoding String");
  }

  bool decodeRequest(REQUEST_TYPE type, const char *path, JsonObject body) {
    logger.println("Decoding Json");
//    if (type == REQ_MQTT && strcmp(path, "homebridge/from/set") == 0) {
    if (type == REQ_MQTT && strcmp(path, "homebridge/from/set") == 0) {
      decodeHomebridgeCommand(body);
    }
  }

  virtual void decodeCommand(unsigned long code) {
  }

  virtual void refresh() {
  }

  virtual void print() {
  }

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