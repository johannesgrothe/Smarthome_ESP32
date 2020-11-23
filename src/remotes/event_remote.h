#pragma once

#include "console_logger.h"
#include "remote.h"
#include <memory.h>
#include "Event.h"

enum class event_type {
  StatusOn, StatusOff, StatusChanged
};

class EventRemote : public Remote {
private:

protected:
  void handleRequest(std::string path, std::string body) override;

  void handleRequest(std::string path, const JsonObject& body) override;


public:

  explicit EventRemote();

  explicit EventRemote(std::shared_ptr<Request_Gadget> gadget);

  void sendEvent(string sender, event_type type);






};