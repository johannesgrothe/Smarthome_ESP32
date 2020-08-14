#pragma once

#include "gadget_remote.h"
#include "../connectors/smarthome_remote_helper.h"

class SmarthomeGadgetRemote : public GadgetRemote {
private:

  bool registerGadget(const char *gadget_name, GadgetType gadget_type, const char *characteristics) override;

  bool removeGadget(const char *gadget_name) override;

protected:

  void handleRequest(const char *path, const char *body) override;

  void handleRequest(const char *path, const JsonObject& body) override;

public:

  SmarthomeGadgetRemote(Request_Gadget *gadget, JsonObject data);

  void
  updateCharacteristic(const char *gadget_name, const char *service, const char *characteristic, int value) override;

};
