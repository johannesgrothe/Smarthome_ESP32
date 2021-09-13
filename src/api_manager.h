#pragma once

#include "api_manager_delegate.h"
#include "connectors/request.h"

class ApiManager {
private:

  ApiManagerDelegate *delegate_;

  static bool pathIsLegal(std::string path);

public:
  explicit ApiManager(ApiManagerDelegate *delegate);

  void handleRequest(std::shared_ptr<Request> req);
};
