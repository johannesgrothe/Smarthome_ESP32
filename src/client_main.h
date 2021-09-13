#pragma once

#include "api_manager_delegate.h"
#include "api_manager.h"

class ClientMain : public ApiManagerDelegate {
private:
  ApiManager api_manager_;

public:
  ClientMain();
};