#pragma once

#include "datatypes.h"
#include "connectors/event.h"
#include "helpers/threadsafe_queue.h"
#include "system_timer.h"

#include <vector>
#include <memory>

class EventManager {
private:
  std::vector<event_map> event_map_;

  std::vector<std::tuple<std::string, unsigned long long>> last_events_;

  ThreadSafeQueue<std::shared_ptr<Event>> out_queue_;

public:
  explicit EventManager(std::vector<event_map> event_data);

  void handleCode(unsigned long code);

  void handleEvent(const std::shared_ptr<Event>& event);

  bool hasEvent();

  std::shared_ptr<Event> getEvent();
};
