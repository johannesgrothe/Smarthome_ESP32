#include <cstring>
#include <Arduino.h>
#include <connectors/serial_connector.h>
#include <connectors/radio_connector.h>
#include <connectors/mqtt_connector.h>
#include "ArduinoJson.h"
#include "colors.h"
#include "connectors/ir_connector.h"
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
enum SH_LAMP_TYPE {
  ON_OFF, BRI_ONLY, CLR_ONLY, CLR_BRI
};

class SH_Gadget : public IR_Connector, public Serial_Connector, public Radio_Connector, public Homebridge_Connector {
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
  }

  bool decodeRequest(REQUEST_TYPE type, const char *path, JsonObject body) {
    if (type == REQ_MQTT && strcmp(path, "homebridge/out") == 0) {
      decodeHomebridgeCommand(body);
    }
  }

  // Homeridge-Connector
  void updateHomebridgeCharacteristic(const char *characteristic, int value) override {
    updateHomebridgeCharakteristicHelper(name, characteristic, value);
  }

  void updateHomebridgeCharacteristic(const char *characteristic, bool value) override {
    updateHomebridgeCharakteristicHelper(name, characteristic, value);
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

class SH_Lamp : public SH_Gadget {
protected:
  float lightness;
  float last_lightness{};
  float default_lightness;
  float min_lightness;

  float saturation;

  float hue;

  SH_LAMP_TYPE type;

public:

  explicit SH_Lamp(JsonObject gadget) :
    SH_Gadget(gadget),
    lightness(100.0),
    default_lightness(100.0),
    min_lightness(35),
    saturation(0),
    hue(0) {
    if (gadget["lamp_type"] != nullptr) {
      type = (SH_LAMP_TYPE) gadget["lamp_type"].as<uint8_t>();
      logger.print("Type: ");
      logger.addln(type);
    } else {
      type = ON_OFF;
      logger.print(LOG_WARN, "No Type found.");
    }

  };

  SH_Lamp(JsonObject gadget, uint8_t lamp_type) :
    SH_Gadget(gadget),
    lightness(100.0),
    default_lightness(100.0),
    min_lightness(35),
    saturation(0),
    hue(0),
    type((SH_LAMP_TYPE) lamp_type) {
    logger.print("Type: ");
    logger.addln(type);
  };

  // Lightness
  void setLightness(float new_lightness) {
    lightness = new_lightness;
    has_changed = true;
  };

  float getLightness() {
    return lightness;
  };

  // Color (RGB)
  void setColor(uint8_t r, uint8_t g, uint8_t b) {
    float hsl[3];
    rgbToHsl(r, g, b, &hsl[0]);
    hue = hsl[SH_CLR_hue];
    saturation = hsl[SH_CLR_saturation];
    lightness = hsl[SH_CLR_lightness];
    has_changed = true;
  };

  uint8_t getColor(uint8_t color_index) {
    uint8_t rgb[3];
    hslToRgb(hue, saturation, lightness, &rgb[0]);
    return rgb[color_index];
  }

  void getColor(uint8_t color_buffer[]) {
    hslToRgb(hue, saturation, lightness, &color_buffer[0]);
  }

  // Hue
  void setHue(float new_hue) {
    hue = new_hue;
    has_changed = true;
  }

  float getHue() {
    return hue;
  }

  // Status
  void toggleStatus() {
    setStatus(!getStatus());
    has_changed = true;
  };

  bool getStatus() {
    return lightness != 0;
  };

  void setStatus(bool new_status) {
    if (new_status == 0) {
      lightness = 0;
    } else {
      if (last_lightness > min_lightness) {
        lightness = last_lightness;
      } else {
        lightness = default_lightness;
      }
    }
    has_changed = true;
  };

  void print() override {
//    Serial.printf("[%s] Status: %d", name, getStatus());
    if (type == CLR_BRI || type == CLR_ONLY) {
//      Serial.printf(", Hue: %.2f, Saturation: %.2f", hue, saturation);
    }
    if (type == CLR_BRI || type == BRI_ONLY) {
//      Serial.printf(", Lightness: %.2f", lightness);
    }
//    Serial.println("");
  }

//  Homebridge_Connector

  void applyHomebridgeCommand(const char *receiver_name, const char *characteristic, int value) override {
    if (strcmp(name, receiver_name) == 0) {
      if (strcmp(characteristic, "On") == 0) {
        setStatus((bool) value);
      } else if (strcmp(characteristic, "Brightness") == 0) {
        setStatus((float) value);
      } else if (strcmp(characteristic, "Hue") == 0) {
        setHue((float) value);
      }
    }
  }

  bool getHomebridgeRegisterStr(char *buffer) override {
    switch (type) {
      case ON_OFF :
        sprintf(buffer, R"({"name": "%s", "service_name": "%s", "service": "Lightbulb"})", name, name);
        break;
      case BRI_ONLY :
        sprintf(buffer,
                R"({"name": "%s", "service_name": "%s", "service": "Lightbulb", "Brightness": "default"})",
                name, name);
        break;
      case CLR_ONLY :
        sprintf(buffer,
                R"({"name": "%s", "service_name": "%s", "service": "Lightbulb", "Hue": "default", "Saturation": "default"})",
                name, name);
        break;
      case CLR_BRI :
        sprintf(buffer,
                R"({"name": "%s", "service_name": "%s", "service": "Lightbulb", "Brightness": "default", "Hue": "default", "Saturation": "default"})",
                name, name);
        break;
      default :
        return false;
    }
    return true;
  };

  bool getHomebridgeUnregisterStr(char *buffer) override {
    snprintf(&buffer[0], HOMEBRIDGE_UNREGISTER_STR_MAX_LEN, R"({"name": "%s"})", name);
    return true;
  }
  // End of Homebridge-Connector

  // Request-Connector
  void decodeCommand(unsigned long code) override {
    logger.printname(name, "Decoding 0x");
    logger.add(code, HEX);
    logger.add(": ");
    const char *method_name = findMethodForCode(code);
    if (method_name != nullptr) {
      if (strcmp(method_name, "toggleStatus") == 0) {
        logger.addln("'toggleStatus'");
        toggleStatus();
      } else if (strcmp(method_name, "'turnOn'") == 0) {
        logger.addln("'turnOn");
        setStatus(true);
      } else if (strcmp(method_name, "'turnOff'") == 0) {
        logger.addln("'turnOff");
        setStatus(false);
      } else {
        logger.add("Found Nothing for '");
        logger.add(method_name);
        logger.addln("'");
      }
    } else {
      logger.addln(" - ");
    }
  }
  // End of Request-Connector
};

#endif