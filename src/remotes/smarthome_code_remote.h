#ifndef __Smarthome_Code_Remote__
#define __Smarthome_Code_Remote__

#include "code_remote.h"

class SmarthomeCodeRemote : public CodeRemote {
protected:
  void sendCodeToRemote(CodeCommand *code) override {
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

  void handleRequest(const char *path, const char *body) override {};

  void handleRequest(const char *path, JsonObject body) override {
    if (strcmp(path, "smarthome/from/code") != 0) { return; }

    if (body["type"] == nullptr) {
      logger.print(LOG_ERR, "Broken Code Request Received: 'type' missing");
      return;
    }

    if (body["code"] == nullptr) {
      logger.print(LOG_ERR, "Broken Code Request Received: 'code' missing");
      return;
    }

    if (body["timestamp"] == nullptr) {
      logger.print(LOG_ERR, "Broken Code Request Received: 'timestamp' missing");
      return;
    }

    auto *newCode = new CodeCommand(stringToCodeType(body["type"].as<const char *>()), body["code"].as<unsigned long>(),
                                    body["timestamp"].as<unsigned long long>());
    handleNewCodeFromRequest(newCode);
  }

  bool handleNewGadget(SH_Gadget *new_gadget) override { return true; }

public:

  explicit SmarthomeCodeRemote(JsonObject data) :
    CodeRemote(data) {};

  SmarthomeCodeRemote(Request_Gadget *gadget, JsonObject data) :
    CodeRemote(gadget, data) {};

};

#endif //__Smarthome_Code_Remote__