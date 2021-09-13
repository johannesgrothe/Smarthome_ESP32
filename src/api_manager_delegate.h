#pragma once

#include "connectors/gadget_meta.h"
#include "connectors/code_command.h"
#include "connectors/event.h"

class ApiManagerDelegate {
private:

public:
  ApiManagerDelegate();

  virtual void handleGadgetUpdate(std::shared_pointer<GadgetMeta> gadget) = 0;

  virtual void handleCode(std::shared_pointer<CodeCommand> code) = 0;

  virtual void handleEvent(std::shared_pointer<Event> event) = 0;
};
