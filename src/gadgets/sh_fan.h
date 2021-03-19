#pragma once

#include "sh_gadget.h"

#define FAN_ROTATION_SPEED_MAX 100

class SH_Fan : public SH_Gadget {
protected:
  uint8_t rotation_speed_;
  uint8_t last_rotation_speed_;
  uint8_t levels_;

  void executeCharacteristicUpdate(CharacteristicIdentifier characteristic, int value) override;

public:

  /**
   * Constructor for the fan
   * @param name Name of the gadget
   * @param levels_count How many levels the fan should operate at
   */
  explicit SH_Fan(std::string name, uint8_t levels_count);

  /**
   * Toggles the status (on/off) of the fan
   */
  void toggleStatus();

  /**
   * Gets the status of the fan
   * @return Whether the fan is on or off
   */
  bool getStatus() const;

  /**
   * Gets which operation level the fan is in
   * @return The operation leven
   */
  uint8_t getLevel() const;

  /**
   * Turns the fan on or off
   * @param status The new status of the fan
   */
  void setStatus(bool status);

  /**
   * Gets the current rotation speed of the fan
   * @return The current rotation speed of the fan
   */
  uint8_t getRotationSpeed() const;

  /**
   * Increases the rotation speed
   */
  void rotationSpeedUp();

  /**
   * Decreases the rotation speed
   */
  void rotationSpeedDown();

  /**
   * Sets a new rotation speed for the fan
   * @param speed The new rotation speed
   */
  void setRotationSpeed(uint8_t speed);

  vector<GadgetCharacteristicSettings> getCharacteristics() override;

  void handleMethodUpdate(GadgetMethod) override;
};
