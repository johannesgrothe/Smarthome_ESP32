#include "catch.hpp"

#include "../src/connectors/serial_gadget.h"

TEST_CASE("Test Serial Gadget", "[Network]") {
  SerialGadget network;

  DynamicJsonDocument payload(400);
  payload["blub"] = 55;
  auto out_req = std::make_shared<Request>("path/out",
                                           1776,
                                           "yolokopter",
                                           "receiver",
                                           payload);

  std::string string_req = "!r_p[path/out]_b[{\"session_id\":1776,\"sender\":\"yolokopter\",\"receiver\":\"receiver\",\"payload\":{\"blub\":55}}]_\n";

  SECTION("Test Constructor") {
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

}