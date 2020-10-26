#include "radio_gadget.h"

Radio_Gadget::Radio_Gadget() :
    Code_Gadget(),
    driver(nullptr) {
}

Radio_Gadget::Radio_Gadget(uint8_t reciever_pin, uint8_t sender_pin) :
    Code_Gadget(),
    driver(nullptr) {
  driver = new RH_ASK(2000, reciever_pin, sender_pin, 5);
  driver->setModeRx();
  code_gadget_is_ready_ = driver->init();
  if (!code_gadget_is_ready_)
    logger.println(LOG_TYPE::ERR, "code gadget not ready");
}

void Radio_Gadget::refresh() {
//  logger.println(".");
//  if (!driver->init())
//    logger.println("init failed");
  if (!code_gadget_is_ready_)
    return;
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);
  if (driver->recv(buf, &buflen)) // Non-blocking
  {
    logger.printfln(LOG_TYPE::INFO, "Recieved this code: %d", buf);
    RH_ASK::printBuffer("msg: ", buf, buflen);
  }
}

void Radio_Gadget::sendRadio(const unsigned long code) {
  uint8_t buf[RH_ASK_MAX_MESSAGE_LEN];
  uint8_t buflen = sizeof(buf);
  unsigned long temp_code = code;
  unsigned long buf_code = code;
  int code_len = 0;
  while (temp_code >= 0xFF & code_len < RH_ASK_MAX_MESSAGE_LEN){
    temp_code = temp_code / 0xFF;
    code_len ++;
  }
  buf[0] = buf_code;
  for (int i = 0; i < code_len; i++){
    buf[i] = buf_code % 0xFF;
    buf_code = buf_code / 0xFF;
  }
  buf[code_len + 1] = (uint8_t) '/0';
  Serial.println("spongo");
  driver->send(buf, buflen);
  driver->waitPacketSent();
  logger.printfln("this code: %d was sent", code);
  for (int i = 0; i < code_len; i++) {
    Serial.println(buf[i]);
  }
  Serial.println();
}

