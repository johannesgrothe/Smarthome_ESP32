#ifndef SERIAL_CONNECTOR_H
#define SERIAL_CONNECTOR_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "code_connector.h"

class Serial_Gadget : public Code_Gadget {
protected:

public:
  Serial_Gadget() :
    Code_Gadget() {
  };

  explicit Serial_Gadget(JsonObject data) :
    Code_Gadget(data) {
    logger.println("Creating Serial Gadget");
    logger.incIntent();
    logger.println(LOG_DATA, "Using default Serial Connection");
    logger.decIntent();
  };

  void refresh() override {
    if (!is_initialized) {
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
      last_command_hex = strtol(&incoming_message[0], NULL, 16);
      if (last_command_hex == 0) {
        strcpy(&last_command_str[0], &incoming_message[0]);
        if (strcmp(last_command_str, "0") != 0) {
          has_news = true;
        }
      } else {
        has_news = true;
      }
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
