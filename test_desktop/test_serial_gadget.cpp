#include "catch.hpp"

#include "../src/connectors/serial_gadget.h"
#include "../src/console_logger.h"
#include "../src/system_timer.h"

TEST_CASE("Test Serial Gadget", "[Network]") {
  SerialGadget network;
  system_timer.lockTime(1000);

  DynamicJsonDocument payload(400);
  payload["blub"] = 55;
  auto out_req = std::make_shared<Request>("path/out",
                                           1776,
                                           "yolokopter",
                                           "receiver",
                                           false,
                                           payload);

  std::string string_req = "!r_p[path/out]_b[{\"session_id\":1776,\"sender\":\"yolokopter\",\"receiver\":\"receiver\",\"is_response\":false,\"payload\":{\"blub\":55}}]_\n";


  auto out_req_split = std::make_shared<Request>("path/out",
                                                 1776,
                                                 "brokenboi",
                                                 "receiver",
                                                 false,
                                                 payload);
  std::string string_req_0 = "!r_p[path/out]_b[{\"session_id\":1776,\"sender\":\"brokenboi\",\"r";
  std::string string_req_1 = "eceiver\":\"receiver\",\"is_response\":false,\"payload\":{\"blub\":55}}]_\n";


  SECTION("Test Constructor") {
    CHECK(*out_req != *out_req_split);
    CHECK(network.getGadgetType() == RequestGadgetType::SERIAL_G);
    CHECK(network.requestGadgetIsReady() == true);
  }

  SECTION("Test Send") {
    network.sendRequest(out_req);
    network.refresh();
    CHECK(Serial.last_print == string_req);
  }

  SECTION("Test Receive") {
    Serial.mockData(string_req);
    network.refresh();
    CHECK(network.hasRequest() == true);
    auto received_req = network.getRequest();
    CHECK(received_req != nullptr);
    CHECK(*out_req == *received_req);
  }

  SECTION("Test Receive Split") {
    Serial.mockData(string_req_0);
    network.refresh();
    CHECK(network.hasRequest() == false);

    system_timer.lockTime(1100);

    Serial.mockData(string_req_1);
    network.refresh();
    CHECK(network.hasRequest() == true);

    auto received_req = network.getRequest();
    CHECK(received_req != nullptr);
    CHECK(*out_req_split == *received_req);
  }

  SECTION("Test Receive Split Break") {
    Serial.mockData(string_req_0);
    network.refresh();
    CHECK(network.hasRequest() == false);

    system_timer.lockTime(1200);

    Serial.mockData(string_req_1);
    network.refresh();
    CHECK(network.hasRequest() == false);

    system_timer.lockTime(1500);

    Serial.mockData(string_req);
    network.refresh();
    CHECK(network.hasRequest() == true);

    auto received_req = network.getRequest();
    CHECK(received_req != nullptr);
    CHECK(*out_req == *received_req);
  }
}
