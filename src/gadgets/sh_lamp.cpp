#include "sh_lamp.h"

SH_Lamp::SH_Lamp(const JsonObject& gadget) :
  SH_Gadget(gadget, GadgetType::Lightbulb),
  lamp_color_(0, 0, 0),
  default_brightness_(75),
  min_brightness_(10),
  last_brightness_(75)
  {
    if (gadget["lamp_type"] != nullptr) {
      lamp_type_ = (SHLampType) gadget["lamp_type"].as<uint8_t>();
      logger.print("Type: ");
      logger.addln(int(type));
    } else {
      lamp_type_ = SHLampType::ON_OFF;
      logger.print(LOG_TYPE::WARN, "No Type found.");
    }
  };

SH_Lamp::SH_Lamp(const JsonObject& gadget, const SHLampType sh_lamp_type) :
  SH_Gadget(gadget, GadgetType::Lightbulb),
  lamp_color_(0, 0, 0),
  default_brightness_(75),
  min_brightness_(10),
  last_brightness_(75),
  lamp_type_(sh_lamp_type) {
    logger.print("Type: ");
    logger.addln(int(type));
  };

  // Lightness
  void SH_Lamp::setBrightness(byte new_brightness) {
    lamp_color_.setBrightness(new_brightness);
    has_changed = true;
    updateCharacteristic("brightness", (int) new_brightness);
  };

  float SH_Lamp::getBrightness() {
    return lamp_color_.getBrightness();
  };

  // Color (RGB)
  void SH_Lamp::setColor(uint8_t r, uint8_t g, uint8_t b) {
    lamp_color_.setRGB(r, g, b);
    has_changed = true;
  };

  uint8_t SH_Lamp::getColor(uint8_t color_index) {
    if (color_index == 0)
      return lamp_color_.getRGB()->getRed();
    if (color_index == 1)
      return lamp_color_.getRGB()->getGreen();
    if (color_index == 2)
      return lamp_color_.getRGB()->getBlue();
    return 0;
  }

  void SH_Lamp::getColor(uint8_t color_buffer[]) {
    color_buffer[0] = lamp_color_.getRGB()->getRed();
    color_buffer[1] = lamp_color_.getRGB()->getGreen();
    color_buffer[2] = lamp_color_.getRGB()->getBlue();
  }

  // Hue
  void SH_Lamp::setHue(unsigned int new_hue) {
    lamp_color_.setHue(new_hue);
    has_changed = true;
    updateCharacteristic("hue", (int) new_hue);
  }

  float SH_Lamp::getHue() {
    return lamp_color_.getHue();
  }

  // Status
  void SH_Lamp::toggleStatus() {
    setStatus(!getStatus());
  };

  bool SH_Lamp::getStatus() {
    return getBrightness() != 0;
  };

  void SH_Lamp::setStatus(bool new_status) {
    if (new_status == 0) {
      lamp_color_.setBrightness(0);
    } else {
      if (last_brightness_ > min_brightness_) {
        lamp_color_.setBrightness(last_brightness_);
      } else {
        lamp_color_.setBrightness(default_brightness_);
      }
    }
    has_changed = true;
    updateCharacteristic("status", new_status);
  };

  void SH_Lamp::print() {
    Serial.printf("[%s] status: %d", getName(), getStatus());
    if (lamp_type_ == SHLampType::CLR_BRI || lamp_type_ == SHLampType::CLR_ONLY) {
      Serial.printf(", hue: %d, saturation: %d", lamp_color_.getHue(), lamp_color_.getHSV()->getSaturation());
    }
    if (lamp_type_ == SHLampType::CLR_BRI || lamp_type_ == SHLampType::BRI_ONLY) {
      Serial.printf(", brightness: %d", lamp_color_.getBrightness());
    }
    Serial.println("");
  }

  void SH_Lamp::handleCharacteristicUpdate(const char *characteristic, int value) {
    logger.print(getName(), "Updating Characteristic: '");
    logger.add(characteristic);
    logger.addln("'");
    if (strcmp(characteristic, "status") == 0) {
      setStatus((bool) value);
    } else if (strcmp(characteristic, "brightness") == 0) {
      setBrightness((byte) value);
    } else if (strcmp(characteristic, "hue") == 0) {
      setHue((unsigned int) value);
    }
  }

  bool SH_Lamp::getCharacteristics(char *characteristic_str) {
    switch (lamp_type_) {
      case SHLampType::ON_OFF :
        return false;
      case SHLampType::BRI_ONLY :
        strcpy(characteristic_str, R"("brightness": "default")");
        break;
      case SHLampType::CLR_ONLY :
        strcpy(characteristic_str, R"("hue": "default", "saturation": "default")");
        break;
      case SHLampType::CLR_BRI :
        strcpy(characteristic_str, R"("brightness": "default", "hue": "default", "saturation": "default")");
        break;
      default :
        return false;
    }
    return true;
  }

  void SH_Lamp::handleMethodUpdate(const char *method) {
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