#ifndef __CodeRemote__
#define __CodeRemote__

#include "ArduinoJson.h"
#include "../connectors/request_gadget.h"
#include "../connectors/mqtt_gadget.h"

#include "../system_settings.h"
#include "../system_timer.h"
#include "remote.h"
#include "../connectors/code_command.h"

#include <climits>
#include <cstring>



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


class CodeCommandBuffer {
private:
  // TODO: Durch C++-Standart-Datenstruktur ersetzen
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

  // TODO: Check if there's any better way
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
        logger.print(LOG_DATA, "");
        logger.add(k);
        logger.add(": ");
        logger.add(code_list[k]->getTimestamp());
        logger.add(" -> ");
        logger.addln(code_list[k]->getCode());
      } else {
        logger.print(LOG_DATA, "");
        logger.add(k);
        logger.add(": ");
        logger.addln("null");
      }
    }
  }

};

class CodeRemote : public Remote {
private:

  CodeCommandBuffer codes;

  void addCodeToBuffer(CodeCommand *code) {
    if (!codes.codeIsDoubled(code)) {
      codes.addCode(code);
      codes.print();
    } else {
      logger.println(LOG_ERR, "Ignoring: Double Code");
    }
  }

  void forwardCodeToGadgets(CodeCommand *code) {
    logger.print("Forwarding Code to ");
    logger.add(gadgets.getGadgetCount());
    logger.addln(" Gadgets:");
    logger.incIndent();
    for (int i = 0; i < gadgets.getGadgetCount(); i++) {
      gadgets[i]->handleCodeUpdate(code->getCode());
    }
    logger.decIndent();
  }

  void forwardAllCodes() {
    while (codes.hasNewCode()) {
      forwardCodeToGadgets(codes.getCode());
    }
  }

protected:

  virtual void sendCodeToRemote(CodeCommand *code) = 0;

public:
  explicit CodeRemote(JsonObject data) :
    Remote(data){};

  CodeRemote(Request_Gadget *gadget, JsonObject data) :
    Remote(gadget, data) {};

  void handleNewCodeFromGadget(CodeCommand *code) {
    addCodeToBuffer(code);
    sendCodeToRemote(code);
    forwardAllCodes();
  }

  void handleNewCodeFromRequest(CodeCommand *code) {
    addCodeToBuffer(code);
    forwardAllCodes();
  }
};

#endif //__CodeRemote__