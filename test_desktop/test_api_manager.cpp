#include "catch.hpp"

#include "../src/api/api_manager.h"
#include "../src/test_dummys/api_manager_delegate_dummy.h"
#include "../src/test_helpers/request_response_listener.h"
#include "../src/api/protocol_paths.h"
#include "../src/test_dummys/request_gadget_dummy.h"
#include "../test_fixtures/api_jsons.h"

#define TEST_CLIENT_ID "test_client"
#define TEST_BRIDGE_ID "test_bridge"
#define TEST_ILLEGAL_PATH "yolokopter"
#define TEST_ILLEGAL_BROADCAST PATH_CLIENT_CONFIG_DELETE
#define TEST_OTHER_CLIENT_ID "not_test_client"

TEST_CASE("Test API Manager", "[API]") {

  ApiManagerDelegateDummy delegate;
  auto network = std::make_shared<RequestGadgetDummy>();
  ApiManager manager(&delegate, network, 1776, TEST_CLIENT_ID);

  SECTION("Test Illegal Path") {
    DynamicJsonDocument payload(400);
    payload["test"] = "yolo";

    RequestResponseListener listener(TEST_ILLEGAL_PATH, TEST_BRIDGE_ID, TEST_CLIENT_ID, payload);

    manager.handleRequest(listener.getRequest());

    CHECK(listener.getResponse() == nullptr);
  }

  SECTION("Test Illegal Broadcast") {
    DynamicJsonDocument payload(400);
    payload["test"] = "yolo";

    RequestResponseListener listener(TEST_ILLEGAL_BROADCAST, TEST_CLIENT_ID, payload);

    manager.handleRequest(listener.getRequest());

    CHECK(listener.getResponse() == nullptr);
  }

  SECTION("Test Request to other Client") {
    DynamicJsonDocument payload(400);
    payload["test"] = "yolo";

    RequestResponseListener listener(TEST_ILLEGAL_BROADCAST, TEST_BRIDGE_ID, TEST_OTHER_CLIENT_ID, payload);

    manager.handleRequest(listener.getRequest());

    CHECK(listener.getResponse() == nullptr);
  }

  SECTION("Test Echo") {
    DynamicJsonDocument payload(400);
    payload["test"] = "yolo";

    RequestResponseListener listener(PATH_TEST_ECHO, TEST_BRIDGE_ID, TEST_CLIENT_ID, payload);

    manager.handleRequest(listener.getRequest());

    CHECK(listener.getResponse() != nullptr);
    CHECK(listener.getResponse()->getPayload() == payload);
  }

  SECTION("Test Echo Broadcast") {
    DynamicJsonDocument payload(400);
    payload["test"] = "yolokopter pew pew";

    RequestResponseListener listener(PATH_TEST_ECHO, TEST_CLIENT_ID, payload);

    manager.handleRequest(listener.getRequest());

    CHECK(listener.getResponse() != nullptr);
    CHECK(listener.getResponse()->getPayload() == payload);
  }

  SECTION("Test Sync") {
    CHECK(network->getLastSentRequest() == nullptr);

    DynamicJsonDocument payload(400);
    auto sync_request = std::make_shared<Request>(PATH_SYNC_REQUEST,
                                                  177787,
                                                  TEST_BRIDGE_ID,
                                                  TEST_CLIENT_ID,
                                                  payload);
    manager.handleRequest(sync_request);
    CHECK(network->getLastSentRequest() != nullptr);
  }

  SECTION("Test Gadget Update") {
    CHECK(delegate.had_gadget_meta == false);

    DynamicJsonDocument payload(400);
    payload["gadget"] = generateGadgetFan();

    auto sync_request = std::make_shared<Request>(PATH_SYNC_GADGET,
                                                  177787,
                                                  TEST_BRIDGE_ID,
                                                  TEST_CLIENT_ID,
                                                  payload);
    manager.handleRequest(sync_request);
    CHECK(delegate.had_gadget_meta == true);
  }

  SECTION("Test Gadget Update Broken") {
    DynamicJsonDocument payload(400);

    auto sync_request = std::make_shared<Request>(PATH_SYNC_GADGET,
                                                  177787,
                                                  TEST_BRIDGE_ID,
                                                  TEST_CLIENT_ID,
                                                  payload);
    manager.handleRequest(sync_request);
    CHECK(delegate.had_gadget_meta == false);
  }

}
