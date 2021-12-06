#include "scheduled_messages_manager.h"

#include <utility>

static const char *TAG = "ScheduledMessagesManager";

ScheduledMessagesManager::ScheduledMessagesManager(std::shared_ptr <ApiManager> api) :
    api_(std::move(api)),
    last_heartbeat_(0) {}

void ScheduledMessagesManager::loop() {
  auto now = system_timer.getTime();
  if (last_heartbeat_ + HEARTBEAT_INTERVAL < now) {
    logger_i(TAG, "Sending heartbeat");
    last_heartbeat_ = now;
    api_->publishHeartbeat();
  }
}
