#pragma once

#include "event_remote.h"

class SmarthomeEventRemote : public EventRemote {
private:

  bool registerGadget(const std::string& gadget_name, GadgetType gadget_type, vector<GadgetCharacteristicSettings> characteristics) override;

  bool removeGadget(const std::string& gadget_name) override;



protected:

  void handleRequest(std::shared_ptr<Request> req) override;

public:

  explicit SmarthomeEventRemote(std::shared_ptr<Request_Gadget> gadget);

  void sendEvent(string sender, EventType type) override;

};
