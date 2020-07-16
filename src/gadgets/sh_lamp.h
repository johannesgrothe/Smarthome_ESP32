#ifndef __SH_Lamp__
#define __SH_Lamp__

#include "sh_gadget.h"
#include "../color.h"

enum SH_LAMP_TYPE {
  ON_OFF, BRI_ONLY, CLR_ONLY, CLR_BRI
};

class SH_Lamp : public SH_Gadget {
protected:
  Color lamp_color_;

  byte default_brightness_;
  byte min_brightness_;
  byte last_brightness_;

  SH_LAMP_TYPE lamp_type_;

public:

  explicit SH_Lamp(JsonObject gadget);

  SH_Lamp(JsonObject gadget, SH_LAMP_TYPE sh_lamp_type);

  // Lightness
  void setBrightness(byte new_brightness);

  float getBrightness();

  // Color (RGB)
  void setColor(uint8_t r, uint8_t g, uint8_t b);

  uint8_t getColor(uint8_t color_index);

  void getColor(uint8_t color_buffer[]);

  // Hue
  void setHue(unsigned int new_hue);

  float getHue();

  // Status
  void toggleStatus();

  bool getStatus();

  void setStatus(bool new_status);

  void print() override;

  void handleCharacteristicUpdate(const char *characteristic, int value) override;

  bool getCharacteristics(char *characteristic_str) override;

  void handleMethodUpdate(const char *method) override;
};

#endif //__SH_Lamp__