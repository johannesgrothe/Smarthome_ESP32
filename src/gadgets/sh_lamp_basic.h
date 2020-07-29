#pragma once

#include "sh_lamp.h"
#include <Arduino.h>

class SH_Lamp_Basic : public SH_Lamp {
private:

  uint8_t pin;

  bool default_state;

public:

  explicit SH_Lamp_Basic(const JsonObject&);

  void refresh() override;
};
