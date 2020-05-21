#ifndef RADIO_CONNECTOR_H
#define RADIO_CONNECTOR_H

#include "code_gadget.h"

class Radio_Gadget : public Code_Gadget {
protected:

public:
  Radio_Gadget() :
    Code_Gadget() {
  };

  explicit Radio_Gadget(JsonObject data) :
    Code_Gadget(data) {};

  void refresh() override {
  };
};

#endif //RADIO_CONNECTOR_H
