#include <cstring>
#include <Arduino.h>
#include "ArduinoJson.h"
#include "colors.h"
#include "connectors/ir_connector.h"

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

class SH_Gadget : public IR_Connector {
protected:
  char name[30]{};
  bool initialized;
  bool has_changed;

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
      strcpy(name, gadget["name"].as<const char *>());
//      name = gadget["name"].as<const char *>();
      Serial.printf("    => Name: %s\n", name);
    } else {
      strcpy(name, "");
      Serial.println("    => [ERR] No Name Found.");
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

  /**
   * @return Whether the Gadget is initialized or not
   */
  virtual bool decode(DynamicJsonDocument *doc) {
    return false;
  }

  virtual bool getRegisterStr(char *buffer) {
    return false;
  }

  virtual void refresh() {
  }

  virtual void print() {
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
      Serial.printf("    => Type: %d\n", type);
    } else {
      type = ON_OFF;
      Serial.println("    => [WARN] No Type Found.");
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
    Serial.printf("    => Type: %d\n", type);
  };

  // Lightness
  void setLightness(float new_lightness) {
    Serial.printf("[%s] Setting Lightness: %f\n", name, new_lightness);
    lightness = new_lightness;
    has_changed = true;
  };

  float getLightness() {
    return lightness;
  };

  // Color (RGB)
  void setColor(uint8_t r, uint8_t g, uint8_t b) {
    Serial.printf("[%s] Setting Color: [%d, %d, %d]\n", name, r, g, b);
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
    Serial.printf("[%s] Setting Hue: %.1f\n", name, new_hue);
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
    Serial.printf("[%s] Setting Status: %d\n", name, new_status);
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

  // Others
  bool decode(DynamicJsonDocument *doc) override {
    JsonObject obj = doc->as<JsonObject>();
    if (obj["characteristic"] == "On") {
      setStatus((bool) obj["value"]);
    } else if (obj["characteristic"] == "Brightness") {
      setLightness((float) obj["value"]);
    } else if (obj["characteristic"] == "Hue") {
      setHue((float) obj["value"]);
    }
    return true;
  };

  bool getRegisterStr(char *buffer) override {
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

  void print() override {
    Serial.printf("[%s] Hue: %.2f, Lightness: %.2f, Saturation: %.2f, Status: %d\n",
                  name,
                  hue,
                  lightness,
                  saturation,
                  getStatus());
  }
};

#endif