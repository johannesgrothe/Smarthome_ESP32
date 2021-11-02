#pragma once

#include <memory>
#include <vector>

#include "gadget_meta.h"
#include "client_meta.h"
#include "../connectors/code_command.h"
#include "../connectors/event.h"
#include "../storage/system_config.h"
#include "../storage/gadget_config.h"
#include "../storage/event_config.h"

class ApiManagerDelegate {
public:

  virtual void handleGadgetUpdate(GadgetMeta gadget) = 0;

  virtual void handleEvent(Event event) = 0;

  virtual ClientMeta getClientData() = 0;

  virtual std::vector<GadgetMeta> getGadgetData() = 0;

  virtual bool handleSystemConfigWrite(SystemConfig cfg) = 0;

  virtual bool handleGadgetConfigWrite(GadgetConfig cfg) = 0;

  virtual bool handleEventConfigWrite(EventConfig cfg) = 0;
};
