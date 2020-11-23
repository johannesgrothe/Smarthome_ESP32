#pragma once

#include <sstream>

class Event {
private:

  std::string sender;
  unsigned long long timestamp;
  unsigned int event_type;

public:

  Event(std::string , unsigned long long , unsigned int );

};

