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

  void executeCharacteristicUpdate(GadgetCharacteristic, int) override;

public:

  /**
   * Constructor for the SH_Lamp
   * @param name Name of the gadget
   * @param lamp_type Type of the lamp
   */
  SH_Lamp(std::string name, SHLampType lamp_type);

  /**
   * Sets the brightness of the lamp
   * @param brightness The new brightness to be set
   */
  void setBrightness(byte brightness);

  /**
   * Gets the brightness of the lamp
   * @return The brightness
   */
  float getBrightness();

  /**
   * Sets the color of the lamp
   * @param red Red-value
   * @param green Green-value
   * @param blue Blue-value
   */
  void setColor(uint8_t red, uint8_t green, uint8_t blue);

  /**
   * Gets just a special color channel
   * @param index Index of the color channel
   * @return The color value
   */
  uint8_t getColor(uint8_t index);

  void getColor(uint8_t color[]);

  void setHue(unsigned int hue);

  /**
   * Gets the color of the lamp
   * @return The color of the lamp
   */
  unsigned int getHue();

  /**
   * Toggles the status (on/off) of the fan
   */
  void toggleStatus();

  /**
   * Gets the status of the fan
   * @return Whether the fan is on or off
   */
  bool getStatus();

  /**
   * Turns the fan on or off
   * @param status The new status of the fan
   */
  void setStatus(bool status);

  vector<GadgetCharacteristicSettings> getCharacteristics() override;

  void handleMethodUpdate(GadgetMethod method) override;
};
