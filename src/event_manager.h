#pragma once

#include "datatypes.h"
#include "connectors/event.h"
#include "helpers/threadsafe_queue.h"

#include <vector>
#include <memory>

class EventManager {
private:
  std::vector<event_map> event_map_;

  ThreadSafeQueue<std::shared_ptr<Event>> out_queue_;

public:
  explicit EventManager(std::vector<event_map> event_data);

  void handleCode(unsigned long code);

  void handleEvent(const std::shared_ptr<Event>& event);

  bool hasEvent();

  std::shared_ptr<Event> getEvent();
};
