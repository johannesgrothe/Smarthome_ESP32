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

class Radio_Connector {
protected:

  Radio_Gadget *radiogadget;

public:
  Radio_Connector() = default;

};

#endif //RADIO_CONNECTOR_H
