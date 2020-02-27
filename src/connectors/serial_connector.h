#ifndef SERIAL_CONNECTOR_H
#define SERIAL_CONNECTOR_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "code_gadget.h"
#include "request_gadget.h"

class Serial_Gadget : public Code_Gadget, public Request_Gadget {
protected:
  bool messageType(const char *message) {
    unsigned long comBuffer = 0;
    short bufflen = strlen(message);
    for (short i = 0; i < bufflen; i++) {
      int charInt = (int) message[i];
      if (!((charInt >= 48 && charInt <= 57) || (charInt >= 65 && charInt <= 70) || (charInt >= 97 && charInt <= 102)))
        return false;
    }
    return true;
  }

public:
  Serial_Gadget() :
      Code_Gadget(),
      Request_Gadget() {
  };

  explicit Serial_Gadget(JsonObject data) :
      Code_Gadget(data),
      Request_Gadget(data) {
    logger.println("Creating Serial Gadget");
    logger.incIntent();
    logger.println(LOG_DATA, "Using default Serial Connection");
    logger.decIntent();
    request_gadget_is_ready = true;
    code_gadget_is_ready = true;
  };

  void refresh() override {

    if (!code_gadget_is_ready || !request_gadget_is_ready) {
      return;
    }
    char incoming_message[30]{};
    uint8_t k = 0;
    bool new_msg = false;
    while (Serial.available() > 0 && k < 30) {
      char buf = Serial.read();
      if (buf != '\n') {
        incoming_message[k] = buf;
        k++;
        new_msg = true;
      }
    }
    if (new_msg) {
      if (messageType(incoming_message)) {
        setCommand(strtol(incoming_message, NULL, 16));
      } else {
        setRequest("String test", incoming_message, REQ_SERIAL);
      }

//      if (comBuffer == 0) {
//        strcpy(&last_command_str[0], &incoming_message[0]);
//        if (strcmp(last_command_str, "0") != 0) {
//          has_news = true;
//        }
//      } else {
//        has_news = true;
//      }
    }
  }
};

class Serial_Connector {
protected:

  Serial_Gadget *serialgadget;

  bool initialized_serial;

public:
  Serial_Connector() :
      serialgadget(nullptr),
      initialized_serial(false) {
  };

  void init_serial_con(Serial_Gadget *new_serial_gadget) {
    initialized_serial = true;
    serialgadget = new_serial_gadget;
  }

};

#endif //SERIAL_CONNECTOR_H
