#include "catch.hpp"

#include "../src/scheduled_messages_manager.h"
#include "../src/test_dummys/request_gadget_dummy.h"
#include "../src/api/protocol_paths.h"

TEST_CASE("Test ScheduledMessagesManager", "[Utility]") {

  auto network = std::make_shared<RequestGadgetDummy>();
  auto api = std::make_shared<ApiManager>(nullptr, network, 5, "api");
  ScheduledMessagesManager manager(api);

  SECTION("Test Heartbeat") {
    system_timer.lockTime(0);
    CHECK(network->getLastSentRequest() == nullptr);
    manager.loop();
    CHECK(network->getLastSentRequest() == nullptr);
    system_timer.lockTime(4999);
    manager.loop();
    CHECK(network->getLastSentRequest() == nullptr);
    system_timer.lockTime(5001);
    manager.loop();
    auto sent_heartbeat = network->getLastSentRequest();
    CHECK(sent_heartbeat != nullptr);
    CHECK(sent_heartbeat->getSender() == "api");
    CHECK(sent_heartbeat->getPath() == api_definitions::uris::heartbeat);
  }
}
