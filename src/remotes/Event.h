#pragma once

#include <sstream>

enum class EventType {
  StatusOn, StatusOff, StatusChanged
};

class Event {
private:

  std::string sender;
  unsigned long long timestamp;
  EventType event_type;

public:

  Event(std::string , unsigned long long , EventType );

  std::string getSender();

  unsigned long long getTimestamp();

  EventType getType();

};

