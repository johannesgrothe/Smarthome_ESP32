#ifndef __SH_FAN__
#define __SH_FAN__

#include "sh_gadget.h"

#define FAN_ROTATION_SPEED_MAX 100

class SH_Fan : public SH_Gadget {
protected:
  byte rotation_speed;
  byte last_rotation_speed;
  byte levels;

public:

  explicit SH_Fan(JsonObject gadget);

  explicit SH_Fan(JsonObject gadget, byte levels_count);

  // Status
  void toggleStatus();

  bool getStatus() const;

  byte getLevel() const;

  void setStatus(bool new_status);

  byte getRotationSpeed() const;

  void rotationSpeedUp();

  void rotationSpeedDown();

  void setRotationSpeed(byte new_speed);

  void print() override;

//  Homebridge-Connector
  void handleCharacteristicUpdate(const char *characteristic, int value) override;

  bool getCharacteristics(char *buffer) override;
  // End of Homebridge-Connector

  // Code-Connector
  void handleMethodUpdate(const char *method) override;
  // End of Code-Connector
};

#endif //__SH_FAN__