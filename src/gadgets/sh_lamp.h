#ifndef __SH_Lamp__
#define __SH_Lamp__

#include "sh_gadget.h"

enum SH_LAMP_TYPE {
  ON_OFF, BRI_ONLY, CLR_ONLY, CLR_BRI
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
    setHomebridgeServiceType("Lightbulb");
  };

  // Lightness
  void setLightness(float new_lightness) {
    lightness = new_lightness;
    has_changed = true;
    updateHomebridgeCharacteristic("Brightness", (int) lightness);
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
    updateHomebridgeCharacteristic("Hue", (int) hue);
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
    updateHomebridgeCharacteristic("Brightness", (int) lightness);
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
  void applyHomebridgeCommand(const char *characteristic, int value) override {
    if (strcmp(characteristic, "On") == 0) {
      setStatus((bool) value);
    } else if (strcmp(characteristic, "Brightness") == 0) {
      setStatus((float) value);
    } else if (strcmp(characteristic, "Hue") == 0) {
      setHue((float) value);
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

#endif //__SH_Lamp__