#pragma once

#include <memory>
#include <vector>

#include "gadget_meta.h"
#include "client_meta.h"
#include "../connectors/code_command.h"
#include "../connectors/event.h"

class ApiManagerDelegate {
private:

public:
//  ApiManagerDelegate();

  virtual void handleGadgetUpdate(std::shared_ptr<GadgetMeta> gadget) = 0;

  virtual void handleCode(std::shared_ptr<CodeCommand> code) = 0;

  virtual void handleEvent(std::shared_ptr<Event> event) = 0;

  virtual std::string getClientId() = 0;

  virtual ClientMeta getClientData() = 0;

  virtual std::vector<GadgetMeta> getGadgetData() = 0;
};
