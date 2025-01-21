#pragma once

#include <vector>

#include "gadget_meta.h"
#include "client_meta.h"
#include "../connectors/event.h"
#include "../storage/system_config.h"
#include "gadget_update_meta.h"

class ApiManagerDelegate {
public:
  virtual ~ApiManagerDelegate() = default;

  virtual void handleGadgetUpdate(GadgetUpdateMeta gadget) = 0;

  virtual void handleEvent(Event event) = 0;

  virtual ClientMeta getClientData() = 0;

  virtual std::vector<GadgetMeta> getGadgetData() = 0;

  virtual bool handleSystemConfigWrite(SystemConfig cfg) = 0;
};
