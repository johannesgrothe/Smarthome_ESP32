#pragma once

#include "gadget_remote.h"
#include "../connectors/smarthome_remote_helper.h"

class SmarthomeGadgetRemote : public GadgetRemote {
private:

  bool registerGadget(const std::string& gadget_name, GadgetType gadget_type, vector<GadgetCharacteristicSettings> characteristics) override;

  bool removeGadget(const std::string& gadget_name) override;

protected:

  void handleRequest(std::shared_ptr<Request> req) override;

public:

  explicit SmarthomeGadgetRemote(std::shared_ptr<Request_Gadget> gadget);

  void
  updateCharacteristic(std::string gadget_name, GadgetCharacteristic characteristic, int value) override;

};
