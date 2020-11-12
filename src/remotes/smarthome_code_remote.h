#pragma once

#include "code_remote.h"

class SmarthomeCodeRemote : public CodeRemote {
protected:

  void sendCodeToRemote(CodeCommand *code) override;

  void handleRequest(std::shared_ptr<Request> req) override;

  bool handleNewGadget(std::shared_ptr<SH_Gadget> new_gadget) override;

public:

  explicit SmarthomeCodeRemote();

  explicit SmarthomeCodeRemote(std::shared_ptr<Request_Gadget> gadget);

};
