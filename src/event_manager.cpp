#include "event_manager.h"

#include <utility>
#include <sstream>

EventManager::EventManager(std::vector<event_map> event_data) :
    event_map_(std::move(event_data)),
    last_events_(),
    out_queue_() {}

void EventManager::handleCode(unsigned long code) {
  for (auto mapping: event_map_) {
    auto event_name = std::get<0>(mapping);
    auto codes = std::get<1>(mapping);
    for (auto found_code: codes) {
      if (found_code == code) {
        std::stringstream strm;
        strm << "Code_" << code;
        auto out_event = std::make_shared<Event>(event_name, strm.str(), system_timer.getTime());
        handleEvent(out_event);
      }
    }
  }
}

void EventManager::handleEvent(const std::shared_ptr<Event> &event) {
  if (event == nullptr) {
    return;
  }

  int replace_index = -1;
  std::tuple<std::string, unsigned long long> add_tuple(event->name, event->timestamp);

  for (int i = 0; i < last_events_.size(); i++) {
    auto event_name = std::get<0>(last_events_[i]);
    if (event_name == event->name) {
      replace_index = i;
      if (std::get<1>(last_events_[i]) + 50 < event->timestamp) {
        out_queue_.push(event);
      }
    }
  }

  if (replace_index >= 0) {
    last_events_[replace_index] = add_tuple;
  } else {
    last_events_.push_back(add_tuple);
    out_queue_.push(event);
  }
}

bool EventManager::hasEvent() {
  return !out_queue_.isEmpty();
}

std::shared_ptr<Event> EventManager::getEvent() {
  return out_queue_.pop();
}
