#pragma once

#include "code_remote.h"

class SmarthomeCodeRemote : public CodeRemote {
protected:

  void sendCodeToRemote(CodeCommand *code) override;

  void handleRequest(std::string path, std::string body) override;

  void handleRequest(std::string path, const JsonObject& body) override;

  bool handleNewGadget(std::shared_ptr<SH_Gadget> new_gadget) override;

public:

  explicit SmarthomeCodeRemote(JsonObject data);

  SmarthomeCodeRemote(Request_Gadget *gadget, JsonObject data);

};
