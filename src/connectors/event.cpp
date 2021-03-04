#include "event.h"

#include <utility>


Event::Event(std::string sender, unsigned long long int timestamp, EventType event_type) :
  sender(std::move(sender)),
  timestamp(timestamp),
  event_type(event_type){}

std::string Event::getSender() const {
    return sender;
}

unsigned long long Event::getTimestamp() const {
    return timestamp;
}

EventType Event::getType() const {
    return event_type;
};
