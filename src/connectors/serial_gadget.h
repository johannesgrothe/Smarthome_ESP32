#ifndef SERIAL_CONNECTOR_H
#define SERIAL_CONNECTOR_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include "code_gadget.h"
#include "request_gadget.h"

class Serial_Gadget : public Code_Gadget, public Request_Gadget {
protected:

  bool strContainsHEX(const char *message) {
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
    Request_Gadget() {};

  explicit Serial_Gadget(JsonObject data) :
    Code_Gadget(data),
    Request_Gadget(data) {
    logger.println("Creating Serial Gadget");
    logger.incIndent();
    logger.println(LOG_DATA, "Using default Serial Connection");
    logger.decIndent();
    request_gadget_is_ready = true;
    code_gadget_is_ready = true;
  };

  void
  sendRequest(REQUEST_TYPE req_type, const char *content_type, IPAddress ip, int port, const char *req_path,
              const char *req_body) override {

  }

  void
  sendRequest(REQUEST_TYPE req_type, const char *content_type, IPAddress ip, int port, const char *req_path,
              JsonObject req_body) override {

  }

  void sendAnswer(const char *req_body, int status_code) override {

  };

  void sendAnswer(JsonObject req_body, int status_code) override {

  };

  void refresh() override {

    if (!code_gadget_is_ready || !request_gadget_is_ready || Serial.available() <= 0) {
      return;
    }
    char incoming_message[REQUEST_BODY_LEN_MAX + REQUEST_PATH_LEN_MAX]{};
    uint8_t k = 0;
    bool new_msg = false;
    while (Serial.available() > 0 && k < REQUEST_BODY_LEN_MAX + REQUEST_PATH_LEN_MAX) {
      char buf = Serial.read();
      if (buf != '\n') {
        incoming_message[k] = buf;
        k++;
        new_msg = true;
      }
    }
    if (new_msg) {
      if (strContainsHEX(incoming_message)) {
        setCommand(strtol(incoming_message, NULL, 16));
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

          setRequest(&message_path[0], &message_body[0], REQ_SERIAL);
        } else {
          setRequest("_unknown_", &incoming_message[0], REQ_SERIAL);
        }
      }
    }
  }

};

#endif //SERIAL_CONNECTOR_H

#if false

void decodeStringCommand(const char *message, unsigned int length) {
    std::string com = message;

    if (com.rfind("_sys:", 0) == 0) {
      logger.print("System Command Detected: ");
      if (com.rfind("_sys:flash", 0) == 0) {
        logger.addln("flash");
//        char input_json[900]{};
      } else if (com.rfind("_sys:reboot", 0) == 0) {
        logger.addln("reboot");
        rebootChip("Input Command");
      } else {
        logger.addln("<unknown>");
      }
    } else if (com.rfind("_dev:", 0) == 0) {
      logger.print("Development Command Detected: ");
      if (com.rfind("_dev:log_on", 4) == 0) {
        logger.addln("log_on");
        logger.activateLogging();
      } else if (com.rfind("_dev:log_off", 4) == 0) {
        logger.addln("log_off");
        logger.deactivateLogging();
      } else {
        logger.addln("<unknown>");
      }
    }
  }

#endif