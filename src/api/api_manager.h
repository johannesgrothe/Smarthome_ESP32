#pragma once

#include "api_manager_delegate.h"
#include "gadget_meta.h"
#include "client_meta.h"
#include "../connectors/request_gadget.h"
#include "../connectors/request.h"
#include "../connectors/code_command.h"
#include "../connectors/event.h"

class ApiManager {
private:

  ApiManagerDelegate *delegate_;

  std::shared_ptr<RequestGadget> network_;

  static bool pathIsLegal(const std::string& path);

  static bool pathIsBroadcast(const std::string& path);

  static void handleEcho(const std::shared_ptr<Request>& req);

  void handleGadgetUpdate(std::shared_ptr<Request> req);

  void handleCodeUpdate(std::shared_ptr<Request> req);

  void handleEventUpdate(std::shared_ptr<Request> req);

public:

  ApiManager(ApiManagerDelegate *delegate, std::shared_ptr<RequestGadget> network);

  void handleRequest(std::shared_ptr<Request> req);

  void publishSync();

  void publishGadgetUpdate(GadgetMeta gadget_data);

  void publishCode(std::shared_ptr<CodeCommand> code);

  void publishEvent(std::shared_ptr<Event> event);
};
