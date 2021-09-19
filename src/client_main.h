#pragma once

#include "api/api_manager_delegate.h"
#include "api/api_manager.h"

class ClientMain : public ApiManagerDelegate {
private:
  ApiManager api_manager_;

public:
  ClientMain();
};