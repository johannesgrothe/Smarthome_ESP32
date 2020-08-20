#pragma once

#include "sh_doorbell.h"

class SH_Doorbell_Basic : public SH_Doorbell {
protected:

  byte switch_pin_;

  bool default_state_;

public:

  explicit SH_Doorbell_Basic(const JsonObject& gadget);

  void refresh() override;
};
