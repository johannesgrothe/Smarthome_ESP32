#ifndef SERIAL_CONNECTOR_H
#define SERIAL_CONNECTOR_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "code_gadget.h"
#include "request_gadget.h"

class Serial_Gadget : public Code_Gadget, public Request_Gadget {
protected:

  static bool strContainsHEX(const char *message) {
    short buff_len = strlen(message);
    for (short i = 0; i < buff_len; i++) {
      int charInt = (int) message[i];
      if (!((charInt >= 48 && charInt <= 57) || (charInt >= 65 && charInt <= 70) || (charInt >= 97 && charInt <= 102)))
        return false;
    }
    return true;
  }

  void executeRequestSending(Request *req) override {
    Serial.printf("_%s:%s\n", req->getPath(), req->getBody());
  };

public:
  Serial_Gadget() :
    Code_Gadget(),
    Request_Gadget() {};

  explicit Serial_Gadget(JsonObject data) :
    Code_Gadget(data),
    Request_Gadget(SERIAL_G, data) {
    if (data.isNull()) {}
    logger.println("Creating Serial Gadget");
    logger.incIndent();
    logger.println(LOG_DATA, "Using default Serial Connection");
    logger.decIndent();
  };

  void refresh() override {
    if (!code_gadget_is_ready_ || !request_gadget_is_ready) {
      return;
    }
    char incoming_message[REQUEST_BODY_LEN_MAX + REQUEST_PATH_LEN_MAX]{};
    unsigned int k = 0;
    bool new_msg = false;
    while (Serial.available() && k < REQUEST_BODY_LEN_MAX + REQUEST_PATH_LEN_MAX) {
      char buf = Serial.read();
      if (buf != '\n') {
        incoming_message[k] = buf;
        k++;
        new_msg = true;
      }
      delayMicroseconds(80);
    }
    if (new_msg) {
      if (strContainsHEX(incoming_message)) {
        setCommand(SERIAL_C, strtoul(incoming_message, NULL, 16));
      } else {
        unsigned int msg_len = strlen(incoming_message);
        if (incoming_message[0] == '_') {
          int msg_pointer = 1;
          char message_path[REQUEST_PATH_LEN_MAX]{};
          char message_body[REQUEST_BODY_LEN_MAX]{};

          int path_pointer = 0;
          while (msg_pointer < msg_len) {
            char buf_char = incoming_message[msg_pointer];
            if (msg_pointer < REQUEST_PATH_LEN_MAX && buf_char != ':') {
              message_path[path_pointer] = buf_char;
              path_pointer++;
            }
            msg_pointer++;
            if (buf_char == ':') {
              break;
            }
          }

          int body_pointer = 0;
          while (msg_pointer < msg_len) {
            char buf_char = incoming_message[msg_pointer];
            message_body[body_pointer] = buf_char;
            body_pointer++;
            msg_pointer++;
          }
          using std::placeholders::_1;
          auto *req = new Request(&message_path[0], &message_body[0], std::bind(&Request_Gadget::sendRequest, this, _1));
          addIncomingRequest(req);
        } else {
          using std::placeholders::_1;
          auto *req = new Request("_unknown_", &incoming_message[0], std::bind(&Request_Gadget::sendRequest, this, _1));
          addIncomingRequest(req);
        }
      }
    }
    sendQueuedItems();
  }

};

#endif //SERIAL_CONNECTOR_H
