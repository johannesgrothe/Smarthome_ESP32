#include "connectors.h"
// Radio Connector

Radio_Connector::Radio_Connector() :
  radio_gadget_(nullptr) {};

void Radio_Connector::setRadio(const std::shared_ptr<Radio_Gadget>& new_radio_gadget) {
  radio_gadget_ = new_radio_gadget;
}

bool Radio_Connector::hasRadio() const {
  return radio_gadget_ != nullptr;
}

//IR Connector

bool IR_Connector::sendRawIR(const uint16_t raw_data[], const uint8_t content_length) {
  if (!hasIR())
    return ir_gadget_->sendRawIR(raw_data, content_length);
  logger.println(LOG_TYPE::ERR, "Cannot send IR: no access.");
  return false;
}

bool IR_Connector::sendIR(const unsigned long command, const uint8_t com_type) {
  if (!hasIR())
    return ir_gadget_->sendIR(command, com_type);
  logger.println(LOG_TYPE::ERR, "Cannot send IR: no access.");
  return false;
}

IR_Connector::IR_Connector() :
  ir_gadget_(nullptr) {};

void IR_Connector::setIR(const std::shared_ptr<IR_Gadget>& new_ir_gadget) {
  ir_gadget_ = new_ir_gadget;
}

bool IR_Connector::hasIR() const {
  return ir_gadget_ != nullptr;
}