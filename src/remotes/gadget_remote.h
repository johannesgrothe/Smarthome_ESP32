#pragma once

#include "ArduinoJson.h"
#include "../connectors/request_gadget.h"

#include "../system_settings.h"
#include "remote.h"

class GadgetRemote: public Remote {
private:

  virtual bool
  registerGadget(const std::string& gadget_name, GadgetType gadget_type, vector<GadgetCharacteristicSettings> characteristics) = 0;

  virtual bool removeGadget(const std::string& gadget_name) = 0;

  bool registerGadgetOnRemote(const string& gadget_name, GadgetType gadget_type, const vector<GadgetCharacteristicSettings>& characteristics);

  bool handleNewGadget(std::shared_ptr<SH_Gadget> new_gadget) override;

public:
  explicit GadgetRemote();

  explicit GadgetRemote(std::shared_ptr<Request_Gadget> gadget);

  virtual void
  updateCharacteristic(std::string gadget_name, GadgetCharacteristic characteristic, int value);

};
