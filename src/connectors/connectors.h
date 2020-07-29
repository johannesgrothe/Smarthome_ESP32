#pragma once

#include "ir_gadget.h"
#include "radio_gadget.h"
#include "console_logger.h"

class Radio_Connector {
private:

  Radio_Gadget *radio_gadget_;
  bool has_radio_;

protected:


public:

  Radio_Connector();

  void setRadio(Radio_Gadget *);

  bool hasRadio() const;

};

class IR_Connector {
private:

  IR_Gadget *ir_gadget_;
  bool has_ir_;

protected:

  bool sendRawIR(const uint16_t [], const uint8_t);

  bool sendIR(const unsigned long, const uint8_t);

public:

  IR_Connector();

  void setIR(IR_Gadget *);

  bool hasIR() const;

};
