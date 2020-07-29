#pragma once

#include "code_remote.h"

class SmarthomeCodeRemote : public CodeRemote {
protected:

  void sendCodeToRemote(CodeCommand *code) override;

  void handleRequest(const char *path, const char *body) override;

  void handleRequest(const char *path, const JsonObject& body) override;

  bool handleNewGadget(SH_Gadget *new_gadget) override;

public:

  explicit SmarthomeCodeRemote(JsonObject data);

  SmarthomeCodeRemote(Request_Gadget *gadget, JsonObject data);

};
