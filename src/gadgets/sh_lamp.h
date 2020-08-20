#pragma once

#include "sh_gadget.h"
#include "../color.h"

enum class SHLampType {
  ON_OFF, BRI_ONLY, CLR_ONLY, CLR_BRI
};

class SH_Lamp : public SH_Gadget {
protected:
  Color lamp_color_;

  byte default_brightness_;
  byte min_brightness_;
  byte last_brightness_;

  SHLampType lamp_type_;

public:

  explicit SH_Lamp(const JsonObject&);

  SH_Lamp(const JsonObject&, SHLampType);

  // Lightness
  void setBrightness(byte);

  float getBrightness();

  // Color (RGB)
  void setColor(uint8_t, uint8_t, uint8_t);

  uint8_t getColor(uint8_t);

  void getColor(uint8_t []);

  // Hue
  void setHue(unsigned int);

  float getHue();

  // Status
  void toggleStatus();

  bool getStatus();

  void setStatus(bool);

  void print() override;

  void handleCharacteristicUpdate(const char *, int) override;

  bool getCharacteristics(char *) override;

  void handleMethodUpdate(const char *) override;
};
