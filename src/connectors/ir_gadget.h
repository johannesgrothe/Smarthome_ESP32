#ifndef __IR_Connector__
#define __IR_Connector__

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

  bool sendRawIR(const uint16_t raw_data[], const uint8_t content_length);

  bool sendIR(const unsigned long command, const uint8_t com_type);
};

#endif