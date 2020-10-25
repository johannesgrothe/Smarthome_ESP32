#pragma once

#include "sh_doorbell.h"
#define HW_BOUNCE_DELAY 50

class SH_Doorbell_Basic : public SH_Doorbell {
protected:

  byte switch_pin_;

  bool default_state_;

  unsigned long last_event_;

public:

  explicit SH_Doorbell_Basic(std::string name, uint8_t pin, bool default_state);

  void refresh() override;
};

std::shared_ptr<SH_Doorbell_Basic> createSHDoorbellBasic(std::string name, pin_set pins, const JsonObject& gadget_data) {
  uint8_t pin = 0;
  bool default_state = false;

  if (pins[0] != 0) {
    pin = pins[0];
  } else {
    logger.println(LOG_TYPE::ERR, "No pin set");
    return nullptr;
  }

  if (gadget_data.containsKey("default_state")) {
    default_state = gadget_data["default_state"].as<bool>();
  } else {
  }

  logger.printfln("Pin: %d", pin);
  logger.printfln("Default State: %d", default_state);

  return std::make_shared<SH_Doorbell_Basic>(name, pin, default_state);
}