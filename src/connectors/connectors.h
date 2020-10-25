#pragma once

#include "ir_gadget.h"
#include "radio_gadget.h"
#include "console_logger.h"
#include <memory>

class Radio_Connector {
private:

  std::shared_ptr<Radio_Gadget> radio_gadget_;

protected:

  //TODO: Build some methods to use radio

public:

  Radio_Connector();

  void setRadio(const std::shared_ptr<Radio_Gadget>&);

  bool hasRadio() const;

};

class IR_Connector {
private:

  std::shared_ptr<IR_Gadget> ir_gadget_;

protected:

  bool sendRawIR(const uint16_t [], uint8_t);

  bool sendIR(unsigned long, uint8_t);

public:

  IR_Connector();

  void setIR(const std::shared_ptr<IR_Gadget>&);

  bool hasIR() const;

};
