#pragma once

#include "console_logger.h"
#include "remote.h"
#include <memory.h>
#include "Event.h"

class EventRemote : public Remote {
private:

  virtual bool registerGadget(const std::string& gadget_name, GadgetType gadget_type, vector<GadgetCharacteristicSettings> characteristics) = 0;

  virtual bool removeGadget(const std::string& gadget_name) = 0;


protected:

  void forwardEvent(std::shared_ptr<Event> event);

public:

  explicit EventRemote();

  explicit EventRemote(std::shared_ptr<Request_Gadget> gadget);

  virtual void sendEvent(string sender, EventType type) = 0;








};