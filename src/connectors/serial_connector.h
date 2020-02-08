#ifndef SERIAL_CONNECTOR_H
#define SERIAL_CONNECTOR_H

#include <Arduino.h>
#include <ArduinoJson.h>

class Serial_Gadget {
protected:

  bool is_initialized;

  long last_command;

  char last_command_str[30]{};

  bool has_news;

public:
  Serial_Gadget() :
    is_initialized(false),
    last_command(0),
    has_news(false) {
//    Serial.begin(115200);
  };

  Serial_Gadget(JsonObject data) :
    is_initialized(true),
    last_command(0),
    has_news(false) {
    Serial.println("   [INFO] Creating Serial Gadget");
//    Serial.begin(115200);
  };

  void refresh() {
    if (!is_initialized) {
      return;
    }
//    delay(10);
    char incoming_message[30]{};
    uint8_t k = 0;
    while (Serial.available() > 0 && k < 30) {
      char buf = Serial.read();
      if (buf != '\n') {
        incoming_message[k] = buf;
        k++;
        has_news = true;
      }
    }
    strcpy(&last_command_str[0], &incoming_message[0]);
  }

  bool hasNewCommand() {
    bool buffer = has_news;
    has_news = false;
    return buffer;
  }

  bool isInitialized() {
    return is_initialized;
  }

  bool hasHexCommand() {
    return (getCommandHEX() != 0);
  }

  unsigned int getCommandHEX() {
    return (strtol(&last_command_str[0], NULL, 16));
  }

  const char * getCommandString() {
    return &last_command_str[0];
  }

  uint8_t getCommandLength() {
    return strlen(&last_command_str[0]);
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
