#define CATCH_CONFIG_MAIN // don't need to repeat this in more than one cpp file

#include "catch.hpp"

#include "../test_fixtures/api_jsons.h"
#include "../src/api/api_decoder.h"


TEST_CASE("Test API Decoder", "[API]") {

  auto c_fanspeed_json = generateCharacteristicFanspeed();
  auto c_status_json = generateCharacteristicStatus();
  auto g_fan_json = generateGadgetFan();

  SECTION("Decode characteristic") {
    auto characteristic_meta = ApiDecoder::decodeCharacteristic(c_fanspeed_json);
    CHECK(characteristic_meta.type == gadget_definitions::CharacteristicIdentifier::fan_speed);
    CHECK(characteristic_meta.min_val == 0);
    CHECK(characteristic_meta.max_val == 100);
    CHECK(characteristic_meta.step_val == 2);
    CHECK(characteristic_meta.steps == 4);
    }

  SECTION("Decode gadget") {
    auto gadget_meta = ApiDecoder::decodeGadget(g_fan_json);
    CHECK(gadget_meta.type == gadget_definitions::GadgetIdentifier::fan_westinghouse_ir);
    CHECK(gadget_meta.name == "test_fan");
    CHECK(gadget_meta.characteristics.size() == 2);
  }

  SECTION("Decode system config") {
    DynamicJsonDocument broken_doc(40);
    CHECK(ApiDecoder::decodeSystemConfig(broken_doc) == nullptr);

    auto decoded_config = ApiDecoder::decodeSystemConfig(generateSystemConfig());
    CHECK(decoded_config != nullptr);
    CHECK(decoded_config->id == "test_client");
    CHECK(decoded_config->network_mode == NetworkMode::MQTT);
    CHECK(decoded_config->ir_recv_pin == 1);
    CHECK(decoded_config->ir_send_pin == 2);
    CHECK(decoded_config->radio_recv_pin == 3);
    CHECK(decoded_config->radio_send_pin == 4);
    CHECK(*(decoded_config->wifi_ssid) == "yolowifi");
    CHECK(*(decoded_config->wifi_pw) == "yolopw");
    CHECK(decoded_config->mqtt_ip->toString() == "192.168.166.44");
    CHECK(*(decoded_config->mqtt_port) == 1884);
    CHECK(*(decoded_config->mqtt_username) == "user");
    CHECK(*(decoded_config->mqtt_password) == "pw");
  }

  SECTION("Decode event config") {
    DynamicJsonDocument broken_doc(40);
    auto decoded_config = ApiDecoder::decodeEventConfig(broken_doc);
    CHECK(decoded_config == nullptr);

    broken_doc.createNestedArray("events");
    decoded_config = ApiDecoder::decodeEventConfig(broken_doc);
    CHECK(decoded_config != nullptr);
    CHECK(decoded_config->event_mapping.empty() == true);

    decoded_config = ApiDecoder::decodeEventConfig(generateEventConfig());
    CHECK(decoded_config != nullptr);
    CHECK(decoded_config->event_mapping.empty() == false);
    auto test_mapping = decoded_config->event_mapping[0];
    CHECK(std::get<0>(test_mapping) == "test_event");
  }

  SECTION("Decode gadget config") {
    DynamicJsonDocument broken_doc(40);
    auto decoded_config = ApiDecoder::decodeGadgetConfig(broken_doc);
    CHECK(decoded_config == nullptr);

    broken_doc.createNestedArray("gadgets");
    decoded_config = ApiDecoder::decodeGadgetConfig(broken_doc);
    CHECK(decoded_config != nullptr);
    CHECK(decoded_config->gadgets.empty() == true);


  }

}
