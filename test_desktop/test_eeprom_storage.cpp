#include "catch.hpp"

#include "../src/storage/eeprom_storage.h"

TEST_CASE("Test EEPROM Storage", "[Storage]") {
  EepromStorage storage;
  EEPROM.clear();

  SECTION("Print Layout/Contend") {
    logger_i("EEPROM Test", EepromStorage::getEepromLayout());
    logger_i("EEPROM_Test", EEPROM.getContent());
  }

  SECTION("Test empty eeprom: System Config") {
    auto cfg = storage.loadSystemConfig();
    CHECK(cfg == nullptr);
  }

  SECTION("Test empty eeprom: Gadget Config") {
    auto cfg = storage.loadGadgetConfig();
    CHECK(cfg != nullptr);
    CHECK(cfg->gadgets.empty());
  }

  SECTION("Test empty eeprom: Event Config") {
    auto cfg = storage.loadEventConfig();
    CHECK(cfg != nullptr);
    CHECK(cfg->event_mapping.empty());
  }

  SECTION("Test writing and loading system config") {
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
    logger_i("EEPROM_Test", "New config checksum: %d", c);

    auto res = storage.saveSystemConfig(cfg);
    CHECK(res == true);
    auto loaded_config = storage.loadSystemConfig();
    CHECK(loaded_config != nullptr);
    CHECK(cfg == *loaded_config);
  }

  SECTION("Test writing and loading gadget config") {
    std::vector<gadget_event_map> mapping = {{"yolo", {{2, 44}, {6, 77}}}, {"yolo2", {{3, 77}, {7, 44}}}};
    gadget_tuple gadget1(2,
                         {false, false, false, false, false, false, false, false},
                         {2, 4, 0, 0, 0},
                         "gadget1",
                         "",
                         mapping);
    GadgetConfig cfg({gadget1});

    auto c = cfg.crc16();
    logger_i("EEPROM_Test", "New config checksum: %d", c);

    auto res = storage.saveGadgetConfig(cfg);
    CHECK(res == true);
    auto loaded_config = storage.loadGadgetConfig();
    CHECK(loaded_config != nullptr);
    CHECK(cfg == *loaded_config);
  }

  SECTION("Test writing and loading event config") {
    event_map map1("fb_1_off", {1223232, 123121234, 546456456});
    event_map map2("fb_1_on", {9789789, 797797897, 7897978789});
    EventConfig cfg({map1, map2});

    auto c = cfg.crc16();
    logger_i("EEPROM_Test", "New config checksum: %d", c);

    auto res = storage.saveEventConfig(cfg);
    CHECK(res == true);
    auto loaded_config = storage.loadEventConfig();
    CHECK(loaded_config != nullptr);
    CHECK(cfg == *loaded_config);
  }
}
