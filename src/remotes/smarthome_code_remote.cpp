#include "smarthome_code_remote.h"

void SmarthomeCodeRemote::sendCodeToRemote(CodeCommand *code) {
  if (!isNetworkInitialised())
    return;
  if (code->getType() == IR_UNKNOWN_C || code->getType() == UNKNOWN_C) {
    return;
  }
  char code_type[CODE_TYPE_NAME_LEN]{};
  codeTypeToString(code->getType(), &code_type[0]);

  char code_str[CODE_STR_LEN_MAX]{};
  unsigned long ident = micros() % 7023;
  snprintf(code_str, CODE_STR_LEN_MAX, R"({"request_id": %lu, "type": "%s", "code": "%lu", "timestamp": %llu})",
           ident, code_type, code->getCode(), code->getTimestamp());
  req_gadget->sendRequest(new Request("smarthome/to/code", code_str));
}

void SmarthomeCodeRemote::handleRequest(const char *path, const char *body) {};

void SmarthomeCodeRemote::handleRequest(const char *path, const JsonObject& body) {
  if (strcmp(path, "smarthome/from/code") != 0) { return; }

  if (body["type"] == nullptr) {
    logger.print(LOG_TYPE::ERR, "Broken Code Request Received: 'type' missing");
    return;
  }

  if (body["code"] == nullptr) {
    logger.print(LOG_TYPE::ERR, "Broken Code Request Received: 'code' missing");
    return;
  }

  if (body["timestamp"] == nullptr) {
    logger.print(LOG_TYPE::ERR, "Broken Code Request Received: 'timestamp' missing");
    return;
  }

  auto *newCode = new CodeCommand(stringToCodeType(body["type"].as<const char *>()), body["code"].as<unsigned long>(),
                                  body["timestamp"].as<unsigned long long>());
  handleNewCodeFromRequest(newCode);
}

bool SmarthomeCodeRemote::handleNewGadget(SH_Gadget *new_gadget) { return true; }

SmarthomeCodeRemote::SmarthomeCodeRemote(JsonObject data) :
  CodeRemote(data) {};

SmarthomeCodeRemote::SmarthomeCodeRemote(Request_Gadget *gadget, JsonObject data) :
  CodeRemote(gadget, data) {}