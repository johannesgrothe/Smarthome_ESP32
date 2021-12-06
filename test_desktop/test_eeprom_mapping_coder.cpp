#include "catch.hpp"

#include "../src/storage/eeprom_gadget_mapping_coder.h"

TEST_CASE("Test EEPROM Manager", "[Storage]") {

  std::vector<gadget_event_map> mapping = {{"yolo", {{2, 44}, {6, 77}}}, {"yolo2", {{3, 77}, {7, 44}}}};
  std::string start_str_data = "<yolo<2;44;6;77>><yolo2<3;77;7;44>>";

  SECTION("Test mapping tuple encoding") {
    std::string converted_data = EepromGadgetMappingCoder::encodeMapping(mapping);
    CHECK(converted_data == start_str_data);
    logger_i("Test Eeprom Mapping Coder", converted_data);
  }

  SECTION("Test mapping tuple encoding / decoding") {

    auto buf_data = EepromGadgetMappingCoder::decodeMapping(start_str_data);
    CHECK(buf_data.size() == 2);
    CHECK(std::get<0>(buf_data[0]) == "yolo");
    CHECK(std::get<0>(buf_data[1]) == "yolo2");
    std::string converted_data = EepromGadgetMappingCoder::encodeMapping(buf_data);

    CHECK(converted_data == start_str_data);
  }
}