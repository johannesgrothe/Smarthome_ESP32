#pragma once

#include "sh_doorbell.h"

class SH_Doorbell_Basic : public SH_Doorbell {
protected:

  byte switch_pin;

  bool default_state;

public:

  explicit SH_Doorbell_Basic(const JsonObject& gadget);

  void refresh() override;
};
