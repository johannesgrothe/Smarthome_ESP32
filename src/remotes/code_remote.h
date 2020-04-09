#ifndef __CodeRemote__
#define __CodeRemote__

#include "ArduinoJson.h"
#include "../connectors/request_gadget.h"

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

public:
  explicit CodeRemote(JsonObject data) {

  }

  void handleRequest(Request *req) {
    Serial.println("handling request");
  };

  void handleNewCode(CodeCommand *code) {
    if (!codes.codeIsDoubled(code)) {
      codes.addCode(code);
      codes.print();
    } else {
      logger.println(LOG_ERR, "Ignoring: Double Code");
    }
  };

  bool hasCode() {
    return codes.hasNewCode();
  }

  CodeCommand *getCode() {
    return codes.getCode();
  }
};

#endif //__CodeRemote__