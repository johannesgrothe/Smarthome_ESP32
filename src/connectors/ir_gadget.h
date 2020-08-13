#pragma once

#include <Arduino.h>
#include <IRrecv.h>
#include <IRsend.h>
#include <IRremoteESP8266.h>
#include <ArduinoJson.h>
#include "code_gadget.h"

// Gadget to send and receive IR-Commands
class IR_Gadget : public Code_Gadget {
protected:

  IRrecv *receiver_;
  IRsend *blaster_;

public:

  IR_Gadget();

  explicit IR_Gadget(JsonObject data);

  void refresh() override;

  bool sendRawIR(const uint16_t raw_data[], uint8_t content_length);

  bool sendIR(unsigned long command, uint8_t com_type);
};
