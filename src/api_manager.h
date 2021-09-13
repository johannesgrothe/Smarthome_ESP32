#pragma once

#include "api_manager_delegate.h"
#include "connectors/request.h"

class ApiManager {
private:

  ApiManagerDelegate *delegate_;

  static bool pathIsLegal(std::string path);

  static void handleEcho(std::shared_ptr<Request> req);

  void handleClientSync(std::shared_ptr<Request> req);

public:
  explicit ApiManager(ApiManagerDelegate *delegate);

  void handleRequest(std::shared_ptr<Request> req);

  void publishSync();
};
