#include "connectors.h"
// Radio Connector

Radio_Connector::Radio_Connector() :
  radio_gadget_(nullptr),
  has_radio_(false) {};

void Radio_Connector::setRadio(Radio_Gadget *new_radio_gadget) {
  if (new_radio_gadget != nullptr) {
//    if (radio_gadget != nullptr) {
//      delete radio_gadget;
//    }
    radio_gadget_ = new_radio_gadget;
    has_radio_ = true;
  }
}

bool Radio_Connector::hasRadio() const {
  return has_radio_;
}

//IR Connector

bool IR_Connector::sendRawIR(const uint16_t raw_data[], const uint8_t content_length) {
  if (has_ir_)
    return ir_gadget_->sendRawIR(raw_data, content_length);
  logger.println(LOG_TYPE::ERR, "Cannot send IR: no access.");
  return false;
}

bool IR_Connector::sendIR(const unsigned long command, const uint8_t com_type) {
  if (has_ir_)
    return ir_gadget_->sendIR(command, com_type);
  logger.println(LOG_TYPE::ERR, "Cannot send IR: no access.");
  return false;
}

IR_Connector::IR_Connector() :
  ir_gadget_(nullptr),
  has_ir_(false) {};

void IR_Connector::setIR(IR_Gadget *new_ir_gadget) {
  if (new_ir_gadget != nullptr) {
//    if (ir_gadget != nullptr) {
//      delete ir_gadget;
//    }
    ir_gadget_ = new_ir_gadget;
    has_ir_ = true;
  }
}

bool IR_Connector::hasIR() const {
  return has_ir_;
}