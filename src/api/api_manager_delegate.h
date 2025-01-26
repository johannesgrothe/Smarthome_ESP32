#pragma once

#include <vector>

#include "dtos/gadget_dto.h"
#include "dtos/client_dto.h"
#include "dtos/gadget_update_dto.h"
#include "../connectors/event.h"
#include "../storage/system_config.h"

class ApiManagerDelegate {
public:
  virtual ~ApiManagerDelegate() = default;

  virtual void handleGadgetUpdate(GadgetUpdateDTO gadget) = 0;

  virtual void handleEvent(Event event) = 0;

  virtual ClientDTO getClientData() = 0;

  virtual std::vector<GadgetDTO> getGadgetData() = 0;

  virtual bool handleSystemConfigWrite(SystemConfig cfg) = 0;
};
