#ifndef __CodeRemote__
#define __CodeRemote__

#include "ArduinoJson.h"
#include "../connectors/request_gadget.h"
#include "../connectors/mqtt_gadget.h"

#include "../system_settings.h"
#include "../system_timer.h"

#include <climits>
#include <cstring>

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
  IR_DENON_C
};

static CodeType stringToCodeType(const char *input) {
  if (strcmp(input, "SERIAL") == 0) {
    return SERIAL_C;
  }
  else if (strcmp(input, "RADIO") == 0) {
    return RADIO_C;
  }
  else if (strcmp(input, "IR_UNKNOWN") == 0) {
    return IR_UNKNOWN_C;
  }
  else if (strcmp(input, "IR_NEC") == 0) {
    return IR_NEC_C;
  }
  else if (strcmp(input, "IR_SONY") == 0) {
    return IR_SONY_C;
  }
  else if (strcmp(input, "IR_RC5") == 0) {
    return IR_RC5_C;
  }
  else if (strcmp(input, "IR_RC6") == 0) {
    return IR_RC6_C;
  }
  else if (strcmp(input, "IR_SAMSUNG") == 0) {
    return IR_SAMSUNG_C;
  }
  else if (strcmp(input, "IR_LG") == 0) {
    return IR_LG_C;
  }
  else if (strcmp(input, "IR_PANASONIC") == 0) {
    return IR_PANASONIC_C;
  }
  else if (strcmp(input, "IR_DENON") == 0) {
    return IR_DENON_C;
  }
  else {
    return UNKNOWN_C;
  }
};

static void codeTypeToString(CodeType code, char *buf) {
  switch (code) {
    case UNKNOWN_C:
      strncpy(buf, "UNKNOWN", CODE_TYPE_NAME_LEN);
      break;
    case SERIAL_C:
      strncpy(buf, "SERIAL", CODE_TYPE_NAME_LEN);
      break;
    case RADIO_C:
      strncpy(buf, "RADIO", CODE_TYPE_NAME_LEN);
      break;
    case IR_UNKNOWN_C:
      strncpy(buf, "IR_UNKNOWN", CODE_TYPE_NAME_LEN);
      break;
    case IR_NEC_C:
      strncpy(buf, "IR_NEC", CODE_TYPE_NAME_LEN);
      break;
    case IR_SONY_C:
      strncpy(buf, "IR_SONY", CODE_TYPE_NAME_LEN);
      break;
    case IR_RC5_C:
      strncpy(buf, "IR_RC5", CODE_TYPE_NAME_LEN);
      break;
    case IR_RC6_C:
      strncpy(buf, "IR_RC6", CODE_TYPE_NAME_LEN);
      break;
    case IR_SAMSUNG_C:
      strncpy(buf, "IR_SAMSUNG", CODE_TYPE_NAME_LEN);
      break;
    case IR_LG_C:
      strncpy(buf, "IR_LG", CODE_TYPE_NAME_LEN);
      break;
    case IR_PANASONIC_C:
      strncpy(buf, "IR_PANASONIC", CODE_TYPE_NAME_LEN);
      break;
    case IR_DENON_C:
      strncpy(buf, "IR_DENON", CODE_TYPE_NAME_LEN);
      break;
    default:
      strncpy(buf, "UNDEFINED", CODE_TYPE_NAME_LEN);
      break;
  }
}

class CodeCommand {
private:
  CodeType type;
  unsigned long code;
  unsigned long long timestamp;

public:
  CodeCommand() :
    type(UNKNOWN_C),
    code(0),
    timestamp(0) {};

  CodeCommand(CodeType command_type, unsigned long code_type, unsigned long long code_timestamp) :
    type(command_type),
    code(code_type),
    timestamp(code_timestamp) {};

  CodeType getType() {
    return type;
  }

  unsigned long getCode() {
    return code;
  }

  unsigned long long getTimestamp() {
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
        unsigned long first = code_list[k]->getTimestamp() / 1000000000;
        unsigned long second = code_list[k]->getTimestamp() % 1000000000;
        Serial.print(first);
        Serial.print(second);
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
  MQTT_Gadget *mqtt_gadget;
  bool network_initialized;

  void addCodeToBuffer(CodeCommand *code) {
    if (!codes.codeIsDoubled(code)) {
      codes.addCode(code);
      codes.print();
    } else {
      logger.println(LOG_ERR, "Ignoring: Double Code");
    }
  }

  void sendCodeToRemote(CodeCommand *code) {
    if (!network_initialized)
      return;
    if (code->getType() == IR_UNKNOWN_C || code->getType() == UNKNOWN_C) {
      return;
    }
    char code_type[CODE_TYPE_NAME_LEN]{};
    codeTypeToString(code->getType(), &code_type[0]);

    char code_str[CODE_STR_LEN_MAX]{};
    unsigned long ident = micros() % 7023;
    snprintf(code_str, CODE_STR_LEN_MAX, R"({"request_id": %lu, "type": "%s", "code": "%lu", "timestamp": %llu})", ident, code_type, code->getCode(), code->getTimestamp());
    mqtt_gadget->sendRequest("smarthome/to/code", code_str);
  }

public:
  explicit CodeRemote(JsonObject data) :
    network_initialized(false) {};

  CodeRemote(JsonObject data, MQTT_Gadget *gadget) :
    mqtt_gadget(gadget),
    network_initialized(true) {};

  void handleRequest(Request *req) {
    DynamicJsonDocument json_file(2048);
    DeserializationError err = deserializeJson(json_file, req->getBody());
    if (err != OK) {
      logger.print(LOG_ERR, "Broken Code Request Received: Invalid JSON");
      return;
    }
    JsonObject json_body = json_file.as<JsonObject>();

    if (json_body["type"] == nullptr) {
      logger.print(LOG_ERR, "Broken Code Request Received: 'type' missing");
      return;
    }

    if (json_body["code"] == nullptr) {
      logger.print(LOG_ERR, "Broken Code Request Received: 'code' missing");
      return;
    }

    if (json_body["timestamp"] == nullptr) {
      logger.print(LOG_ERR, "Broken Code Request Received: 'timestamp' missing");
      return;
    }

    CodeCommand *newCode = new CodeCommand(stringToCodeType(json_body["type"].as<const char *>()), json_body["code"].as<unsigned long>(), json_body["timestamp"].as<unsigned long long>());
    handleNewCode(newCode);

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