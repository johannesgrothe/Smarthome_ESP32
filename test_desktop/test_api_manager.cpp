#include "catch.hpp"

#include "../src/api/api_manager.h"
#include "../src/test_dummys/api_manager_delegate_dummy.h"
#include "test_helpers/request_response_listener.h"
#include "../src/api/protocol_paths.h"
#include "../src/test_dummys/request_gadget_dummy.h"
#include "../test_fixtures/api_jsons.h"

#define TEST_CLIENT_ID "test_client"
#define TEST_BRIDGE_ID "test_bridge"
#define TEST_ILLEGAL_PATH "yolokopter"
#define TEST_OTHER_CLIENT_ID "not_test_client"

constexpr auto TEST_ILLEGAL_BROADCAST = api_definitions::uris::client_config_delete;

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

    RequestResponseListener listener(api_definitions::uris::test_echo, TEST_BRIDGE_ID, TEST_CLIENT_ID, payload);

    manager.handleRequest(listener.getRequest());

    CHECK(listener.getResponse() != nullptr);
    CHECK(listener.getResponse()->getPayload() == payload);
  }

  SECTION("Test Echo Broadcast") {
    DynamicJsonDocument payload(400);
    payload["test"] = "yolokopter pew pew";

    RequestResponseListener listener(api_definitions::uris::test_echo, TEST_CLIENT_ID, payload);

    manager.handleRequest(listener.getRequest());

    CHECK(listener.getResponse() != nullptr);
    CHECK(listener.getResponse()->getPayload() == payload);
  }

  SECTION("Test Sync") {
    CHECK(network->getLastSentRequest() == nullptr);

    DynamicJsonDocument payload(400);
    auto sync_request = std::make_shared<Request>(api_definitions::uris::sync_request,
                                                  177787,
                                                  TEST_BRIDGE_ID,
                                                  TEST_CLIENT_ID,
                                                  payload);
    manager.handleRequest(sync_request);
    CHECK(network->getLastSentRequest() != nullptr);
  }

  SECTION("Test Gadget Update") {
    CHECK(delegate.had_gadget_meta == false);

    DynamicJsonDocument payload = generateGadgetFanUpdate();

    auto sync_request = std::make_shared<Request>(api_definitions::uris::update_gadget,
                                                  177787,
                                                  TEST_BRIDGE_ID,
                                                  TEST_CLIENT_ID,
                                                  payload);
    manager.handleRequest(sync_request);
    CHECK(delegate.had_gadget_meta == true);
  }

  SECTION("Test Gadget Update Broken") {
    DynamicJsonDocument payload(400);

    auto request = std::make_shared<Request>(api_definitions::uris::update_gadget,
                                             177787,
                                             TEST_BRIDGE_ID,
                                             TEST_CLIENT_ID,
                                             payload);
    manager.handleRequest(request);
//    CHECK(delegate.had_gadget_meta == false);
// TODO: at the moment, the api manager has no checks and always generates a valid update meta. this is dangerous, please fix
  }

  SECTION("Test remove config") {
    CHECK(network->getLastSentRequest() == nullptr);

    DynamicJsonDocument payload(100);
    RequestResponseListener req(api_definitions::uris::client_config_delete,
                                TEST_BRIDGE_ID,
                                TEST_CLIENT_ID,
                                payload);

    manager.handleRequest(req.getRequest());
//    CHECK(network->getLastSentRequest() != nullptr);  // TODO: implement resetting config
  }

  SECTION("Test write system config broken") {
    CHECK(network->getLastSentRequest() == nullptr);

    DynamicJsonDocument payload(100);
    RequestResponseListener req(api_definitions::uris::client_system_config_write,
                                TEST_BRIDGE_ID,
                                TEST_CLIENT_ID,
                                payload);

    manager.handleRequest(req.getRequest());
    auto resp = req.getResponse();
    CHECK(resp != nullptr);
    CHECK(resp->getPayload()["ack"] == false);
  }

  SECTION("Test write system config") {
    CHECK(network->getLastSentRequest() == nullptr);

    DynamicJsonDocument payload(2000);
    RequestResponseListener req(api_definitions::uris::client_system_config_write,
                                TEST_BRIDGE_ID,
                                TEST_CLIENT_ID,
                                payload);

    manager.handleRequest(req.getRequest());
    auto resp = req.getResponse();
//    CHECK(resp != nullptr);
//    CHECK(resp->getPayload()["ack"] == true);  // TODO: implement test [#71]
  }

  SECTION("Test write gadget config broken") {
    CHECK(network->getLastSentRequest() == nullptr);

    DynamicJsonDocument payload(100);
    RequestResponseListener req(api_definitions::uris::client_gadget_config_write,
                                TEST_BRIDGE_ID,
                                TEST_CLIENT_ID,
                                payload);

    manager.handleRequest(req.getRequest());
    auto resp = req.getResponse();
    CHECK(resp != nullptr);
    CHECK(resp->getPayload()["ack"] == false);
  }

  SECTION("Test write gadget config") {
    CHECK(network->getLastSentRequest() == nullptr);

    DynamicJsonDocument payload(2000);
    RequestResponseListener req(api_definitions::uris::client_gadget_config_write,
                                TEST_BRIDGE_ID,
                                TEST_CLIENT_ID,
                                payload);

    manager.handleRequest(req.getRequest());
    auto resp = req.getResponse();
//    CHECK(resp != nullptr);
//    CHECK(resp->getPayload()["ack"] == true);  // TODO: implement test [#71]
  }

  SECTION("Test write event config broken") {
    CHECK(network->getLastSentRequest() == nullptr);

    DynamicJsonDocument payload(100);
    RequestResponseListener req(api_definitions::uris::client_event_config_write,
                                TEST_BRIDGE_ID,
                                TEST_CLIENT_ID,
                                payload);

    manager.handleRequest(req.getRequest());
    auto resp = req.getResponse();
    CHECK(resp != nullptr);
    CHECK(resp->getPayload()["ack"] == false);
  }

  SECTION("Test write event config") {
    CHECK(network->getLastSentRequest() == nullptr);

    DynamicJsonDocument payload(2000);
    RequestResponseListener req(api_definitions::uris::client_event_config_write,
                                TEST_BRIDGE_ID,
                                TEST_CLIENT_ID,
                                payload);

    manager.handleRequest(req.getRequest());
    auto resp = req.getResponse();
//    CHECK(resp != nullptr);
//    CHECK(resp->getPayload()["ack"] == true);  // TODO: implement test [#71]
  }

}
