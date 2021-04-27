#pragma once

#include "sh_lamp.h"
#include <Arduino.h>

class SH_Lamp_Basic : public SH_Lamp {
private:

  uint8_t pin_;

public:

  explicit SH_Lamp_Basic(std::string name, uint8_t pin);

  void refresh() override;
};

static std::shared_ptr<SH_Lamp_Basic> createSHLampBasic(std::string name, pin_set pins, const JsonObject& gadget_data) {
  uint8_t pin = 0;

  if (pins[0] != 0) {
    pin = pins[0];
  } else {
    logger.log("LampBasic", LOG_TYPE::ERR) << "No pin set.\n";
    return nullptr;
  }

  return std::make_shared<SH_Lamp_Basic>(name, pin);
}