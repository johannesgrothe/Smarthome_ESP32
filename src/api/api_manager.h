#pragma once

#include <memory>
#include "api_manager_delegate.h"
#include "gadget_meta.h"
#include "client_meta.h"
#include "../connectors/request_gadget.h"
#include "../connectors/request.h"
#include "../connectors/code_command.h"
#include "../connectors/event.h"
#include "../random.h"

class ApiManager {
private:

  // Name of the client to be identified in the network
  const std::string client_id_;

  // Runtime id, number generated at startup to identify reboots to network partners
  const uint16_t runtime_id_;

  ApiManagerDelegate *delegate_;

  std::shared_ptr<RequestGadget> network_;

  static uint16_t genRequestID();

  static bool pathIsLegal(const std::string& path);

  static bool pathIsBroadcast(const std::string& path);

  static void handleEcho(const std::shared_ptr<Request>& req);

  void handleGadgetUpdate(const std::shared_ptr<Request>& req);

  void handleEventUpdate(const std::shared_ptr<Request>& req);

  void handleConfigWrite(const std::shared_ptr<Request>& req);

  void handleConfigReset(const std::shared_ptr<Request>& req);

public:

  ApiManager(ApiManagerDelegate *delegate,
             std::shared_ptr<RequestGadget> network,
             uint16_t runtime_id,
             std::string client_identifier);

  void handleRequest(const std::shared_ptr<Request>& req);

  void publishSync(std::string *receiver);

  void publishGadgetUpdate(const GadgetMeta& gadget_data);

  void publishEvent(const std::shared_ptr<Event>& event);

  void publishHeartbeat();
};
