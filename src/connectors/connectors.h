#ifndef __Connectors__
#define __Connectors__

#include "ir_gadget.h"
#include "radio_gadget.h"

class Radio_Connector {
private:

  Radio_Gadget *radio_gadget;
  bool has_radio;

protected:


public:

  Radio_Connector() :
    radio_gadget(nullptr),
    has_radio(false) {};

  void setRadio(Radio_Gadget *new_radio_gadget) {
    if (new_radio_gadget != nullptr) {
      radio_gadget = new_radio_gadget;
      has_radio = true;
    }
  }

  bool hasRadio() {
    return has_radio;
  }

};

class IR_Connector {
private:

  IR_Gadget *ir_gadget;
  bool has_ir;

protected:

  bool sendRawIR(uint16_t raw_data[], uint8_t content_length) {
    if (has_ir)
      return ir_gadget->sendRawIR(raw_data, content_length);
    logger.println(LOG_ERR, "Cannot send: no access.");
    return false;
  }

  bool sendIR(unsigned long command, uint8_t com_type) {
    if (has_ir)
      return ir_gadget->sendIR(command, com_type);
    logger.println(LOG_ERR, "Cannot send: no access.");
    return false;
  }

public:

  IR_Connector() :
    ir_gadget(nullptr),
    has_ir(false) {};

  void setIR(IR_Gadget *new_ir_gadget) {
    if (new_ir_gadget != nullptr) {
      ir_gadget = new_ir_gadget;
      has_ir = true;
    }
  }

  bool hasIR() {
    return has_ir;
  }

};

#endif // __Connectors__