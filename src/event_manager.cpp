#include "event_manager.h"

#include <utility>

EventManager::EventManager(std::vector<event_map> event_data) :
    event_map_(std::move(event_data)) {}

void EventManager::handleCode(unsigned long code) {

}

void EventManager::handleEvent(const std::shared_ptr<Event>& event) {

}

bool EventManager::hasEvent() {
  return false;
}

std::shared_ptr<Event> EventManager::getEvent() {
  return nullptr;
}
