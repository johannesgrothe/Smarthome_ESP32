#pragma once

#include "sh_gadget.h"

#define FAN_ROTATION_SPEED_MAX 100

class SH_Fan : public SH_Gadget {
protected:
  byte rotation_speed_;
  byte last_rotation_speed_;
  byte levels_;

public:

  explicit SH_Fan(const JsonObject&);

  explicit SH_Fan(const JsonObject&, byte);

  // Status
  void toggleStatus();

  bool getStatus() const;

  byte getLevel() const;

  void setStatus(bool);

  byte getRotationSpeed() const;

  void rotationSpeedUp();

  void rotationSpeedDown();

  void setRotationSpeed(byte);

  void print() override;

//  Homebridge-Connector
  void handleCharacteristicUpdate(const char *, int) override;

  bool getCharacteristics(char *) override;
  // End of Homebridge-Connector

  // Code-Connector
  void handleMethodUpdate(const char *) override;
  // End of Code-Connector
};
