#pragma once

#include <sstream>

/**
 * Possible event types
 */
enum class EventType {
  StatusOn, StatusOff, StatusChanged
};

/**
 * Class that represents an event thrown by the gadgets.
 * Covers all updates not covered by the characteristic updates.
 */
class Event {
private:

  // The sender of the Event
  std::string sender;

  // The timestamp of the event
  unsigned long long timestamp;

  // The type of the event
  EventType event_type;

public:

  /**
   * Constructor for the Event
   * @param sender The sender of the event
   * @param timestamp The timestamp of the event
   * @param type What type of event should be created
   */
  Event(std::string sender, unsigned long long timestamp, EventType type);

  /**
   * Gets the sender of the Event
   * @return The sender of the event
   */
  std::string getSender() const;

  /**
   * Gets the timestamp of the event
   * @return The timestamp of the event
   */
  unsigned long long getTimestamp() const;

  /**
   * Gets the type of the event
   * @return The type of the event
   */
  EventType getType() const;
};

