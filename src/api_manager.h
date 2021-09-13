#pragma once

#include "api_manager_delegate.h"
#include "connectors/request.h"

class ApiManager {
private:

  std::shared_pointer <ApiManagerDelegate> delegate_;

public:
  explicit ApiManager(std::shared_pointer<ApiManagerDelegate> delegate);

  void receive(std::shared_pointer<Request> req);
};
