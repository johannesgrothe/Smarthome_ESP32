#ifndef __SH_Lamp__
#define __SH_Lamp__

#include "sh_gadget.h"
#include "../color.h"

enum SH_LAMP_TYPE {
  ON_OFF, BRI_ONLY, CLR_ONLY, CLR_BRI
};

class SH_Lamp : public SH_Gadget {
protected:
  Color lamp_color;

  byte default_brightness;
  byte min_brightness;
  byte last_brightness;

  SH_LAMP_TYPE lamp_type;

public:

  explicit SH_Lamp(JsonObject gadget) :
    SH_Gadget(gadget, Lightbulb),
    lamp_color(0,0,0),
    default_brightness(75),
    min_brightness(10),
    last_brightness(75)
    {
    if (gadget["lamp_type"] != nullptr) {
      lamp_type = (SH_LAMP_TYPE) gadget["lamp_type"].as<uint8_t>();
      logger.print("Type: ");
      logger.addln(type);
    } else {
      lamp_type = ON_OFF;
      logger.print(LOG_WARN, "No Type found.");
    }
  };

  SH_Lamp(JsonObject gadget, SH_LAMP_TYPE sh_lamp_type) :
    SH_Gadget(gadget, Lightbulb),
    lamp_color(0,0,0),
    default_brightness(75),
    min_brightness(10),
    last_brightness(75),
    lamp_type(sh_lamp_type) {
    logger.print("Type: ");
    logger.addln(type);
  };

  // Lightness
  void setBrightness(byte new_brightness) {
    lamp_color.setBrightness(new_brightness);
    has_changed = true;
    updateCharacteristic("Brightness", (int) new_brightness);
  };

  float getBrightness() {
    return lamp_color.getBrightness();
  };

  // Color (RGB)
  void setColor(uint8_t r, uint8_t g, uint8_t b) {
    lamp_color.setRGB(r, g, b);
    has_changed = true;
  };

  uint8_t getColor(uint8_t color_index) {
    if (color_index == 0)
      return lamp_color.getRGB()->getRed();
    if (color_index == 1)
      return lamp_color.getRGB()->getGreen();
    if (color_index == 2)
      return lamp_color.getRGB()->getBlue();
  return 0;
  }

  void getColor(uint8_t color_buffer[]) {
    color_buffer[0] = lamp_color.getRGB()->getRed();
    color_buffer[1] = lamp_color.getRGB()->getGreen();
    color_buffer[2] = lamp_color.getRGB()->getBlue();
  }

  // Hue
  void setHue(unsigned int new_hue) {
    lamp_color.setHue(new_hue);
    has_changed = true;
    updateCharacteristic("Hue", (int) new_hue);
  }

  float getHue() {
    return lamp_color.getHue();
  }

  // Status
  void toggleStatus() {
    setStatus(!getStatus());
  };

  bool getStatus() {
    return getBrightness() != 0;
  };

  void setStatus(bool new_status) {
    if (new_status == 0) {
      lamp_color.setBrightness(0);
    } else {
      if (last_brightness > min_brightness) {
        lamp_color.setBrightness(last_brightness);
      } else {
        lamp_color.setBrightness(default_brightness);
      }
    }
    has_changed = true;
    updateCharacteristic("On", new_status);
  };

  void print() override {
    Serial.printf("[%s] Status: %d", getName(), getStatus());
    if (lamp_type == CLR_BRI || lamp_type == CLR_ONLY) {
      Serial.printf(", Hue: %.2f, Saturation: %.2f", lamp_color.getHue(), lamp_color.getHSV()->getSaturation());
    }
    if (lamp_type == CLR_BRI || lamp_type == BRI_ONLY) {
      Serial.printf(", Brightness: %.2f", lamp_color.getBrightness());
    }
    Serial.println("");
  }

  void handleCharacteristicUpdate(const char *characteristic, int value) override {
    logger.print(getName(), "Updating Characteristic: '");
    logger.add(characteristic);
    logger.addln("'");
    if (strcmp(characteristic, "On") == 0) {
      setStatus((bool) value);
    } else if (strcmp(characteristic, "Brightness") == 0) {
      setBrightness((byte) value);
    } else if (strcmp(characteristic, "Hue") == 0) {
      setHue((unsigned int) value);
    }
  }

  bool getCharacteristics(char *characteristic_str) override {
    switch (type) {
      case ON_OFF :
        return false;
      case BRI_ONLY :
        strcpy(characteristic_str, R"("Brightness": "default")");
        break;
      case CLR_ONLY :
        strcpy(characteristic_str, R"("Hue": "default", "Saturation": "default")");
        break;
      case CLR_BRI :
        strcpy(characteristic_str, R"("Brightness": "default", "Hue": "default", "Saturation": "default")");
        break;
      default :
        return false;
    }
    return true;
  }

  virtual void handleMethodUpdate(const char *method) override {
    if (method != nullptr) {
      if (strcmp(method, "toggleStatus") == 0) {
        toggleStatus();
      } else if (strcmp(method, "turnOn") == 0) {
        setStatus(true);
      } else if (strcmp(method, "turnOff") == 0) {
        setStatus(false);
      }
    }
  }
};

#endif //__SH_Lamp__