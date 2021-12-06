#pragma once

#include <string>

/**
 * Class that represents an event thrown by the gadgets.
 * Covers all updates not covered by the characteristic updates.
 */
class Event {
public:

  // The name of the event
  const std::string name;

  // The sender of the Event
  const std::string sender;

  // The timestamp of the event
  const unsigned long long timestamp;

  /**
   * Constructor for the Event
   * @param name Name/ID of the event
   * @param sender The sender of the event
   * @param timestamp The timestamp of the event
   */
  Event(std::string name, std::string sender, unsigned long long timestamp);
};
