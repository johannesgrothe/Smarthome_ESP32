#include "catch.hpp"

#include "../src/client_main.h"

TEST_CASE("Test Client Main", "[Main]") {

  SystemConfig system_cfg("test_client",
                          NetworkMode::MQTT,
                          0,
                          1,
                          2,
                          3,
                          std::make_shared<std::string>("wifi_ssid"),
                          std::make_shared<std::string>("wifi_password"),
                          std::make_shared<IPContainer>(192, 168, 178, 444),
                          std::make_shared<uint16_t>(4),
                          std::make_shared<std::string>("username"),
                          std::make_shared<std::string>("password"));
  GadgetConfig gadget_cfg({});
  EventConfig event_cfg({});
  auto main = ClientMain(BootMode::Serial_Ony, system_cfg, gadget_cfg, event_cfg);

  SECTION("Test") {
  }
}
