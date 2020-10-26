#pragma once

#include "code_gadget.h"
#include <RH_ASK.h>
#include <SPI.h>

class Radio_Gadget : public Code_Gadget {
private:
  RH_ASK *driver;

public:
  Radio_Gadget();

  explicit Radio_Gadget(uint8_t reciever_pin, uint8_t sender_pin);

  /**
 * refreshes the Radio_Gadget and recieves codes
 */
  void refresh() override;

  /**
 * method for transmitting radio codes to gadgets
 * @param code radio code to transmit
 */
  void sendRadio(unsigned long code);
};
