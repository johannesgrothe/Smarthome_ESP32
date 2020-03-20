#ifndef __SH_Gadget__
#define __SH_Gadget__

#include <cstring>
#include <Arduino.h>
#include "ArduinoJson.h"
#include "../colors.h"
#include "../helping_structures.h"
#include "../system_settings.h"
#include "../console_logger.h"
#include "../connectors/connectors.h"

enum Gadget_Type {
  None, Lightbulb, Fan, Doorbell
};

enum SH_RGB_Color {
  SH_CLR_red, SH_CLR_green, SH_CLR_blue
};

enum SH_HSL_Color {
  SH_CLR_hue, SH_CLR_saturation, SH_CLR_lightness
};

class SH_Gadget : public IR_Connector, public Radio_Connector {
private:

  std::function<void(const char *, const char *, const char *, int)> updateRemotes;

  bool remoteInitialized;

  char name[GADGET_NAME_LEN_MAX]{};

  byte mapping_count{};

  Mapping_Reference *mapping[MAPPING_MAX_COMMANDS]{};

protected:

  bool initialized;

  bool has_changed;

  Gadget_Type type;

  void updateCharacteristic(const char *characteristic, int value) {
    updateRemotes(&name[0], &name[0], characteristic, value);
  }

  const char *findMethodForCode(unsigned long code) {
    for (byte k = 0; k < mapping_count; k++) {
      if (mapping[k]->containsCode(code)) {
        const char *method_name = mapping[k]->getName();
        logger.print(name, "-> ");
        logger.addln(method_name);
        return method_name;
      }
    }
    return nullptr;
  }

  virtual void handleMethodUpdate(const char *method) {}

public:
  SH_Gadget() :
    remoteInitialized(false),
    name("default"),
    initialized(false),
    has_changed(true),
    type(None) {};

  explicit SH_Gadget(JsonObject gadget, Gadget_Type gadget_type) :
    remoteInitialized(false),
    initialized(false),
    has_changed(true),
    type(gadget_type) {
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
      logger.incIndent();
      byte j = 0;
      for (auto &&com : local_mapping) {
        if (j < mapping_count) {
          const char *new_name = com.key().c_str();
          JsonArray buf_arr = com.value().as<JsonArray>();
          mapping[j] = new Mapping_Reference(buf_arr, new_name);
          j++;
        }
      }
      logger.decIndent();
      logger.println("Method Mapping loaded.");
    } else {
      logger.println(LOG_WARN, "No Mapping Found.");
    }
  };

  void initRemoteUpdate(std::function<void(const char *, const char *, const char *, int)> update_method) {
    updateRemotes = update_method;
    logger.println("Initialized Callbacks");
    remoteInitialized = true;
  }

  Gadget_Type getType() { return type; };

  const char *getName() { return &name[0]; };

  virtual bool getCharacteristics(char *characteristic_str) = 0;

  bool isInitialized() { return initialized; };

  void handleCodeUpdate(unsigned long code) {
    const char *method_name = findMethodForCode(code);
    logger.incIndent();
    handleMethodUpdate(method_name);
    logger.decIndent();
  }

  virtual void handleCharacteristicUpdate(const char *characteristic, int value) = 0;

  virtual void refresh() = 0;

  virtual void print() = 0;

  void printMapping() {
    logger.print(name, "Accessible Methods: ");
    logger.addln(mapping_count);
    logger.incIndent();
    for (byte k = 0; k < mapping_count; k++) {
      mapping[k]->printMapping();
    }
    logger.decIndent();
  }

};

#endif //__SH_GAGDET__