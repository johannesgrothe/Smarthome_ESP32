#pragma once

#include "gadget_remote.h"
#include "../connectors/smarthome_remote_helper.h"

class SmarthomeGadgetRemote : public GadgetRemote {
private:

  bool registerGadget(const std::string& gadget_name, GadgetType gadget_type, const char *characteristics) override;

  bool removeGadget(const std::string& gadget_name) override;

protected:

  void handleRequest(std::string path, std::string body) override;

  void handleRequest(std::string path, const JsonObject& body) override;

public:

  explicit SmarthomeGadgetRemote(std::shared_ptr<Request_Gadget> gadget);

  void
  updateCharacteristic(const char *gadget_name, const char *service, const char *characteristic, int value) override;

};
