#pragma once

#include "lamp_onoff.h"
#include "simple_hardware_gadget.h"

class Lamp_Basic : public Lamp_OnOff, public SimpleHardwareGadget {
private:

  uint8_t pin_;

public:

  explicit Lamp_Basic(std::string name, uint8_t pin);

  void executeCharacteristicUpdate(gadget_definitions::CharacteristicIdentifier characteristic, uint16_t step_value) override;

  void refresh() override;
};

static std::shared_ptr<Lamp_Basic> createSHLampBasic(std::string name, port_set pins, const JsonObject& gadget_data) {
  if (pins[0] != 0) {
    return std::make_shared<Lamp_Basic>(name, pins[0]);
  } else {
    logger_e("LampBasic", "No pin set.");
    return nullptr;
  }
}