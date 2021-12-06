#include "catch.hpp"

#include "../src/storage/system_config.h"

TEST_CASE("Test System Config", "[]") {
  auto cfg = SystemConfig("test_client",
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

  SECTION("Test constructor") {
    CHECK(cfg.id == "test_client");
  }

  SECTION("Test == and !=") {
    auto cfg2 = SystemConfig("test_client2",
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
    CHECK(cfg == cfg);
    CHECK(cfg != cfg2);
  }
}
