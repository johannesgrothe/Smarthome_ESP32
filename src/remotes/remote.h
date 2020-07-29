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

  Request_Gadget *req_gadget;
  Gadget_Collection gadgets;

  bool updatesAreLocked() const;

  void lockUpdates();

  void unlockUpdates();

  virtual void handleRequest(const char *path, const char *body) = 0;

  virtual void handleRequest(const char *path, const JsonObject& body) = 0;

  virtual bool handleNewGadget(SH_Gadget *new_gadget) = 0;

  bool isNetworkInitialised();

public:
  explicit Remote(JsonObject data);

  explicit Remote(Request_Gadget *gadget, JsonObject data);

  void handleRequest(Request *req);

  void addGadget(SH_Gadget *new_gadget);
};
