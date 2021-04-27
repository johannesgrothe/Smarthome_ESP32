#pragma once

#include "sh_wallswitch.h"

class SH_Wallswitch_Basic : public SH_Wallswitch {
private:

  uint8_t pin_;
  unsigned long last_action_;
  bool default_state_;

public:

  SH_Wallswitch_Basic(std::string name, uint8_t pin, bool default_state);

  void refresh() override;
};

static std::shared_ptr<SH_Wallswitch_Basic>
createSHWallswitchBasic(std::string name, pin_set pins, const JsonObject &gadget_data) {
  uint8_t pin = 0;
  bool default_state = false;

  if (pins[0] != 0) {
    pin = pins[0];
  } else {
    logger.log("WallswitchBasic", LOG_TYPE::ERR) << "No pin set\n";
    return nullptr;
  }

  if (gadget_data.containsKey("default_state")) {
    default_state = gadget_data["default_state"].as<bool>();
  }

  logger.log("WallswitchBasic", LOG_TYPE::ERR) << "Pin: " << pin << "\n";
  logger.log("WallswitchBasic", LOG_TYPE::ERR) << "Default State: " << default_state << "\n";

  return std::make_shared<SH_Wallswitch_Basic>(name, pin, default_state);
}