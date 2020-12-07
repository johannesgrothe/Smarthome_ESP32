#include "Event.h"

#include <utility>


Event::Event(std::string sender, unsigned long long int timestamp, EventType event_type) :
  sender(std::move(sender)),
  timestamp(timestamp),
  event_type(event_type){};
