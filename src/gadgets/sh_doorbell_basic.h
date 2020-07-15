#ifndef __SH_Doorbell_Basic__
#define __SH_Doorbell_Basic__

#include "sh_doorbell.h"

class SH_Doorbell_Basic : public SH_Doorbell {
protected:

  byte switch_pin;

  bool default_state;

public:

  explicit SH_Doorbell_Basic(JsonObject gadget);

  void refresh() override;
};

#endif //__SH_Doorbell_Basic__