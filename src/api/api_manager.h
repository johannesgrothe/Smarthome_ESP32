#pragma once

#include <memory>
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

  void handleGadgetUpdate(const std::shared_ptr<Request>& req);

  void handleCodeUpdate(const std::shared_ptr<Request>& req);

  void handleEventUpdate(const std::shared_ptr<Request>& req);

public:

  ApiManager(ApiManagerDelegate *delegate, std::shared_ptr<RequestGadget> network);

  void handleRequest(const std::shared_ptr<Request>& req);

  void publishSync(std::string *receiver);

  void publishGadgetUpdate(const GadgetMeta& gadget_data);

  void publishCode(const std::shared_ptr<CodeCommand>& code);

  void publishEvent(const std::shared_ptr<Event>& event);
};
