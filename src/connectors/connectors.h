#ifndef __Connectors__
#define __Connectors__

#include "ir_gadget.h"
#include "radio_gadget.h"

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

  bool sendRawIR(uint16_t [], uint8_t);

  bool sendIR(unsigned long, uint8_t);

public:

  IR_Connector();

  void setIR(IR_Gadget *);

  bool hasIR() const;

};

#endif // __Connectors__