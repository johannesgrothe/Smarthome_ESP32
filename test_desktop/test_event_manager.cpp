#include "catch.hpp"

#include "../src/event_manager.h"


TEST_CASE("Test Event Manager", "[Main]") {
  std::tuple<std::string, std::vector<unsigned long>> mapping1("test_event", {123, 456});
  EventManager manager({mapping1});

  SECTION("Test empty") {
    CHECK(manager.hasEvent() == false);
    CHECK(manager.getEvent() == nullptr);
  }

  SECTION("Test non existent code") {
    manager.handleCode(111);
    CHECK(manager.hasEvent() == false);
    CHECK(manager.getEvent() == nullptr);
  }

  SECTION("Test code handling") {
    system_timer.lockTime(1776);
    manager.handleCode(123);

    // Manager has event and event is the right one
    CHECK(manager.hasEvent() == true);
    auto event = manager.getEvent();
    CHECK(event != nullptr);
    CHECK(event->timestamp == 1776);
    CHECK(event->name == "test_event");

    // Manager now has no event anymore
    CHECK(manager.hasEvent() == false);
    CHECK(manager.getEvent() == nullptr);

    system_timer.lockTime(1789);
    manager.handleCode(456);

    // Last Event was too short ago, no new event recorded
    CHECK(manager.hasEvent() == false);
    CHECK(manager.getEvent() == nullptr);

    system_timer.lockTime(1850);
    manager.handleCode(456);

    // New event recorded after enough time has passed
    CHECK(manager.hasEvent() == true);
    event = manager.getEvent();
    CHECK(event != nullptr);
    CHECK(event->timestamp == 1850);
    CHECK(event->name == "test_event");
  }

  SECTION("Test event nullptr") {
    manager.handleEvent(nullptr);

    // manager has no event after passing nullptr
    CHECK(manager.hasEvent() == false);
    CHECK(manager.getEvent() == nullptr);
  }
}
