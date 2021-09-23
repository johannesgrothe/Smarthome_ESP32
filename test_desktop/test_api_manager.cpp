#include <stdexcept>
#include <cassert>

#include "../src/api/api_manager.h"
#include "../src/test_dummys/api_manager_delegate_dummy.h"
#include "../src/test_helpers/request_response_listener.h"
#include "../src/api/protocol_paths.h"
#include "../src/test_dummys/request_gadget_dummy.h"

#define TEST_CLIENT_ID "test_client"
#define TEST_BRIDGE_ID "test_bridge"

void test_api_manager() {
  auto delegate = ApiManagerDelegateDummy(TEST_CLIENT_ID);
  auto manager = ApiManager(&delegate, nullptr);
}

void test_api_manager_echo() {
  ApiManagerDelegateDummy delegate(TEST_CLIENT_ID);
  auto network = std::make_shared<RequestGadgetDummy>();
  ApiManager manager(&delegate, network);

  DynamicJsonDocument payload(400);
  payload["test"] = "yolo";

  RequestResponseListener listener(PATH_ECHO_TEST, TEST_BRIDGE_ID, TEST_CLIENT_ID, payload);

  manager.handleRequest(listener.getRequest());

  assert(listener.getResponse() != nullptr);
  assert(listener.getResponse()->getPayload() == payload);
}


// Test function
int main () {
  test_api_manager();
  test_api_manager_echo();
}
