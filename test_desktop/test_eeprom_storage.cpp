#include "catch.hpp"

#include "../src/storage/eeprom_storage.h"

TEST_CASE("Test EEPROM Storage", "[Storage]") {
  EepromStorage storage;
  EEPROM.clear();
  logger_i("EEPROM_Test", EEPROM.getContent());

  SECTION("Print Layout") {
    logger_i("EEPROM Test", EepromStorage::getEepromLayout());
  }

  SECTION("Test empty eeprom") {
    auto cfg = storage.loadSystemConfig();
    CHECK(cfg == nullptr);
  }

  SECTION("Test writing and loading config") {
    SystemConfig cfg("test_client",
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

    auto c = cfg.crc16();
    logger_i("EEPROM_Test", "Checksum: %d", c);

    auto res = storage.saveSystemConfig(cfg);
    CHECK(res == true);
    logger_i("EEPROM_Test", EEPROM.getContent());
    CHECK(storage.loadSystemConfig() != nullptr);
  }
}
