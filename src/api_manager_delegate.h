#pragma once

#include "memory"

#include "connectors/gadget_meta.h"
#include "connectors/code_command.h"
#include "connectors/event.h"

class ApiManagerDelegate {
private:

public:
  ApiManagerDelegate();

  virtual void handleGadgetUpdate(std::shared_ptr<GadgetMeta> gadget) = 0;

  virtual void handleCode(std::shared_ptr<CodeCommand> code) = 0;

  virtual void handleEvent(std::shared_ptr<Event> event) = 0;

  virtual std::string getClientId() = 0;
};
