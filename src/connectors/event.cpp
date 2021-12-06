#include "event.h"

#include <utility>


Event::Event(std::string name, std::string sender, unsigned long long timestamp) :
  name(std::move(name)),
  sender(std::move(sender)),
  timestamp(timestamp) {}
