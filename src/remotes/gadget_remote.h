#pragma once

#include "ArduinoJson.h"
#include "../connectors/request_gadget.h"

#include "../system_settings.h"
#include "remote.h"

class GadgetRemote: public Remote {
private:

  virtual bool
  registerGadget(const char *gadget_name, GadgetType gadget_type, const char *characteristics);

  virtual bool removeGadget(const char *gadget_name);

  bool registerGadgetOnRemote(const char *gadget_name, GadgetType gadget_type, const char *characteristics);

  bool handleNewGadget(SH_Gadget * new_gadget) override;

public:
  explicit GadgetRemote(JsonObject data);

  GadgetRemote(Request_Gadget *gadget, JsonObject data);

  virtual void
  updateCharacteristic(const char *gadget_name, const char *service, const char *characteristic, int value);

};
