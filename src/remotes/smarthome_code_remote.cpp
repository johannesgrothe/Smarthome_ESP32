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
//  req_gadget->sendRequest(new Request("smarthome/to/code", code_str));  // TODO: fix
}

void SmarthomeCodeRemote::handleRequest(std::string path, std::string body) {};

void SmarthomeCodeRemote::handleRequest(std::shared_ptr<Request> req) {
  if (path != "smarthome/from/code") { return; }

  if (req["type"] == nullptr) {
    logger.print(LOG_TYPE::ERR, "Broken Code Request Received: 'type' missing");
    return;
  }

  if (req["code"] == nullptr) {
    logger.print(LOG_TYPE::ERR, "Broken Code Request Received: 'code' missing");
    return;
  }

  if (req["timestamp"] == nullptr) {
    logger.print(LOG_TYPE::ERR, "Broken Code Request Received: 'timestamp' missing");
    return;
  }

  auto *newCode = new CodeCommand(stringToCodeType(req["type"].as<const char *>()), req["code"].as<unsigned long>(),
                                  req["timestamp"].as<unsigned long long>());
  handleNewCodeFromRequest(newCode);
}

bool SmarthomeCodeRemote::handleNewGadget(std::shared_ptr<SH_Gadget> new_gadget) { return true; }

SmarthomeCodeRemote::SmarthomeCodeRemote() :
  CodeRemote() {};

SmarthomeCodeRemote::SmarthomeCodeRemote(std::shared_ptr<Request_Gadget> gadget) :
  CodeRemote(gadget) {}