#include <stdexcept>
#include <cassert>

#include "../src/console_logger.h"
#include "../src/connectors/event.h"

void test_event() {
  Event event("spongo", 7948573847, EventType::StatusOn);
  assert(event.getSender() == "spongo");
  assert(event.getType() == EventType::StatusOn);
  assert(event.getTimestamp() == 7948573847 );
}

int main () {
  logger_i("UnitEvent", "started testing the Event class..." );
  test_event();
  logger_i("UnitEvent", "test successful");
}