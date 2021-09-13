#pragma once

#include "api_manager_delegate.h"
#include "api_manager.h"

class ClientMain(private ApiManagerDelegate) {
private:
  ApiManager api_manager_;

public:
  ClientMain();
};