#ifndef __Radio_Gadget__
#define __Radio_Gadget__

#include "code_gadget.h"

class Radio_Gadget : public Code_Gadget {
protected:

public:
  Radio_Gadget();

  explicit Radio_Gadget(JsonObject);

  void refresh() override;
};

#endif //__Radio_Gadget__
