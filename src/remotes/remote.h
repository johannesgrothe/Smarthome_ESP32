#pragma once

#include "ArduinoJson.h"
#include "../connectors/request_gadget.h"

#include "../system_settings.h"
#include "../gadgets/sh_gadget.h"
#include "../gadget_collection.h"

class Remote {
private:

  bool lock_updates;
  bool network_initialized;

protected:

  std::shared_ptr<Request_Gadget> req_gadget;
  Gadget_Collection gadgets;

  bool updatesAreLocked() const;

  void lockUpdates();

  void unlockUpdates();

  virtual void handleRequest(std::shared_ptr<Request> req) = 0;

  virtual bool handleNewGadget(std::shared_ptr<SH_Gadget> new_gadget) = 0;

  bool isNetworkInitialised() const;

public:
  explicit Remote();

  explicit Remote(std::shared_ptr<Request_Gadget> gadget);

  void handleRequest(Request *req);

  void addGadget(const std::shared_ptr<SH_Gadget>& new_gadget);
};
