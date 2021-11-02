#pragma once

#include "api/api_manager.h"
#include "system_timer.h"

#define HEARTBEAT_INTERVAL 5000

class ScheduledMessagesManager {
private:
  std::shared_ptr<ApiManager> api_;

  unsigned long long last_heartbeat_;

public:
  explicit ScheduledMessagesManager(std::shared_ptr<ApiManager> api);

  void loop();
};
