#ifndef __CodeRemote__
#define __CodeRemote__

#include "ArduinoJson.h"
#include "../connectors/request_gadget.h"
#include "../connectors/rest_gadget.h"

#include "../system_settings.h"
#include "../system_timer.h"

enum CodeType {
  UNKNOWN_C,
  SERIAL_C,
  RADIO_C,
  IR_UNKNOWN_C,
  IR_NEC_C,
  IR_SONY_C,
  IR_RC5_C,
  IR_RC6_C,
  IR_SAMSUNG_C,
  IR_LG_C,
  IR_PANASONIC_C,
  IR_DENEON_C
};

class CodeCommand {
private:
  CodeType type;
  unsigned long code;
  unsigned long timestamp;

public:
  CodeCommand() :
    type(UNKNOWN_C),
    code(0),
    timestamp(0) {};

  CodeCommand(CodeType command_type, unsigned long code_type, unsigned long code_timestamp) :
    type(command_type),
    code(code_type),
    timestamp(code_timestamp) {};

  bool getType() {
    return type;
  }

  unsigned long getCode() {
    return code;
  }

  unsigned long getTimestamp() {
    return timestamp;
  }
};

class CodeCommandBuffer {
private:
  CodeCommand *code_list[CODE_BUFFER_SIZE]{};
  byte buffer_add_pointer{};
  byte buffer_get_pointer{};

  static byte nextPos(byte start) {
    start++;
    if (start == CODE_BUFFER_SIZE)
      start = 0;
    return start;
  }

public:
  CodeCommandBuffer() :
    buffer_add_pointer(0),
    buffer_get_pointer(0) {}

  void addCode(CodeCommand *com) {
    bool pointers_differ = hasNewCode();
    if (code_list[buffer_add_pointer] != nullptr) {
      delete code_list[buffer_add_pointer];
    }
    code_list[buffer_add_pointer] = com;
    buffer_add_pointer = nextPos(buffer_add_pointer);
    if (pointers_differ && !hasNewCode()) {
      buffer_get_pointer++;
    }
  }

  bool hasNewCode() {
    return buffer_get_pointer != buffer_add_pointer;
  }

  CodeCommand *getCode() {
    if (!hasNewCode())
      return nullptr;
    CodeCommand *com = code_list[buffer_get_pointer];
    buffer_get_pointer = nextPos(buffer_get_pointer);
    return com;
  }

  bool codeIsDoubled(CodeCommand *com) {
    for (byte k = 0; k < CODE_BUFFER_SIZE; k++) {
      if (code_list[k] != nullptr) {
        if (code_list[k]->getType() == com->getType() && code_list[k]->getCode() == com->getCode()) {
          if (code_list[k]->getTimestamp() + CODE_TIME_GAP > com->getTimestamp()) {
            return true;
          }
        }
      }
    }
    return false;
  }

  void print() {
    for (byte k = 0; k < CODE_BUFFER_SIZE; k++) {
      if (code_list[k] != nullptr) {
        Serial.print(k);
        Serial.print(": ");
        Serial.print(code_list[k]->getTimestamp());
        Serial.print(" -> ");
        Serial.println(code_list[k]->getCode());
      } else {
        Serial.print(k);
        Serial.print(": ");
        Serial.println("null");
      }
    }
  }

};

class CodeRemote {
protected:
  CodeCommandBuffer codes;
  REST_Gadget *rest_gadget;
  bool network_initialized;
  IPAddress remote_ip;
  unsigned int remote_port;

  void addCodeToBuffer(CodeCommand *code) {
    if (!codes.codeIsDoubled(code)) {
      codes.addCode(code);
//      codes.print();
    } else {
      logger.println(LOG_ERR, "Ignoring: Double Code");
    }
  }

  void sendCodeToRemote(CodeCommand *code) {
    if (!network_initialized)
      return;
    RestRequest req(REQ_HTTP_POST, "/command", "test", remote_port, remote_ip, "application/json");
    rest_gadget->sendRequest(&req);
  }

public:
  explicit CodeRemote(JsonObject data):
  network_initialized(false) {};

  CodeRemote(JsonObject data, REST_Gadget *gadget):
    rest_gadget(gadget) {
    bool everything_ok = true;
    // Reads the IP from the JSON
    if (data["ip"] != nullptr) {
      char ip_str[15]{};
      strncpy(ip_str, data["ip"].as<const char *>(), 15);
      unsigned int ip_arr[4];
      uint8_t count = 0;
      char *part;
      part = strtok(ip_str, ".");
      ip_arr[count] = atoi(part);
      while (count < 3) {
        part = strtok(nullptr, ".");
        count++;
        ip_arr[count] = atoi(part);
      }
      remote_ip = IPAddress(ip_arr[0], ip_arr[1], ip_arr[2], ip_arr[3]);
      logger.print(LOG_DATA, "IP: ");
      logger.add(ip_arr[0]);
      logger.add(".");
      logger.add(ip_arr[1]);
      logger.add(".");
      logger.add(ip_arr[2]);
      logger.add(".");
      logger.addln(ip_arr[3]);
    } else {
      everything_ok = false;
      logger.println(LOG_ERR, "'ip' missing in config.");
    }

    // Reads the Port from the JSON
    if (data["port"] != nullptr) {
      remote_port = data["port"].as<unsigned int>();
      logger.print(LOG_DATA, "Port: ");
      logger.addln(remote_port);
    } else {
      everything_ok = false;
      logger.println(LOG_ERR, "'port' missing in config.");
    }
    network_initialized = everything_ok;
  };

  void handleRequest(Request *req) {
    Serial.println("handling request");
  }

  void handleNewCode(CodeCommand *code) {
    addCodeToBuffer(code);
    sendCodeToRemote(code);
  }

  bool hasCode() {
    return codes.hasNewCode();
  }

  CodeCommand *getCode() {
    return codes.getCode();
  }
};

#endif //__CodeRemote__