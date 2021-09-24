#include "catch.hpp"

#include "../src/api/api_manager.h"
#include "../src/test_dummys/api_manager_delegate_dummy.h"
#include "../src/test_helpers/request_response_listener.h"
#include "../src/api/protocol_paths.h"
#include "../src/test_dummys/request_gadget_dummy.h"

#define TEST_CLIENT_ID "test_client"
#define TEST_BRIDGE_ID "test_bridge"

TEST_CASE("Test API Manager", "[API]") {

  ApiManagerDelegateDummy delegate(TEST_CLIENT_ID);
  auto network = std::make_shared<RequestGadgetDummy>();
  ApiManager manager(&delegate, network);

  SECTION("Test Echo") {
    DynamicJsonDocument payload(400);
    payload["test"] = "yolo";

    RequestResponseListener listener(PATH_ECHO_TEST, TEST_BRIDGE_ID, TEST_CLIENT_ID, payload);

    manager.handleRequest(listener.getRequest());

    CHECK(listener.getResponse() != nullptr);
    CHECK(listener.getResponse()->getPayload() == payload);
  }

}
