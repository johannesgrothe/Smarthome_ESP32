#include "catch.hpp"

#include "../src/client_main.h"
#include "../src/test_dummys/dummy_serial.h"

TEST_CASE("Test Client Main", "[Main]") {

  SystemConfig system_cfg("test_client",
                          NetworkMode::Serial,
                          0,
                          1,
                          2,
                          3,
                          std::make_shared<std::string>("wifi_ssid"),
                          std::make_shared<std::string>("wifi_password"),
                          std::make_shared<IPContainer>(192, 168, 178, 214),
                          std::make_shared<uint16_t>(4),
                          std::make_shared<std::string>("username"),
                          std::make_shared<std::string>("password"));

  std::vector<gadget_event_map> mapping = {{"yolo", {{2, 44}, {6, 77}}}, {"yolo2", {{3, 77}, {7, 44}}}};
  gadget_tuple gadget1(2,
                       {false, false, false, false, false, false, false, false},
                       {2, 4, 0, 0, 0},
                       "gadget1",
                       "",
                       mapping);

  GadgetConfig gadget_cfg({gadget1});

  EventConfig event_cfg({});
  auto main = ClientMain(BootMode::Full_Operation, system_cfg, gadget_cfg, event_cfg);

  SECTION("Test Loop Gadgets") {
    for (int i = 0; i < 5000; i++) {
      main.loopGadgets();
    }
  }

  SECTION("Test Loop System") {
    for (int i = 0; i < 5000; i++) {
      main.loopSystem();
    }
  }

  SECTION("Test Loop Network") {
    for (int i = 0; i < 5000; i++) {
      main.loopNetwork();
    }
    Serial.mockData("yolokopter");
    main.loopNetwork();
  }
}
