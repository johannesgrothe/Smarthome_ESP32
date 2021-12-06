#include "catch.hpp"

#include "../src/mapping_reference.h"

TEST_CASE("Test Mapping Reference", "[Gadget]") {
  // TODO: switch mapping_reference() to std::string from const char*
  char json[100];
  char name;
  const size_t CAPACITY = JSON_ARRAY_SIZE(4);
  StaticJsonDocument<CAPACITY> doc;
  JsonArray arr = doc.to<JsonArray>();
  arr.add(1);
  arr.add(2);
  arr.add(3);
  arr.add(4);
  serializeJson(doc, json);
  Mapping_Reference ref(arr, "default");
  CHECK(strcmp("default", ref.getName()) == 0);
  CHECK(ref.getCodeCount() == 4);
  CHECK(ref.containsCode(3));
}
