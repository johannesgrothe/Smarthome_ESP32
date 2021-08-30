#include <stdexcept>
#include <cassert>

#include "../src/mapping_reference.h"

void test_mapping_reference() {
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
  assert(strcmp("default", ref.getName()) == 0);
  assert(ref.getCodeCount() == 4);
  assert(ref.containsCode(3));
}

int main () {
  logger_i("test_mapping_reference", "started testing of mapping_reference..." );
  test_mapping_reference();
  logger_i("test_mapping_reference", "test successful");
}