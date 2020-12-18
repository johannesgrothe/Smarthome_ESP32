#include "event.h"

#include <utility>


Event::Event(std::string sender, unsigned long long int timestamp, EventType event_type) :
  sender(std::move(sender)),
  timestamp(timestamp),
  event_type(event_type){}

std::string Event::getSender() {
    return sender;
}

unsigned long long Event::getTimestamp() {
    return timestamp;
}

EventType Event::getType() {
    return event_type;
};
