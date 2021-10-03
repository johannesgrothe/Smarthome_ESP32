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

  virtual void handleGadgetUpdate(GadgetMeta gadget) = 0;

  virtual void handleCode(CodeCommand code) = 0;

  virtual void handleEvent(Event event) = 0;

  virtual std::string getClientId() = 0;

  virtual ClientMeta getClientData() = 0;

  virtual std::vector<GadgetMeta> getGadgetData() = 0;
};
