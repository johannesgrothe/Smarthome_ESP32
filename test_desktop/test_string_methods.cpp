#include <stdexcept>
#include <cassert>

#include "../src/console_logger.h"
#include "../src/string_methods.h"

void test_validate_ut8() {
  assert(validate_utf8("spongo"));
}

int main () {
  logger_i("test_validate_ut8", "started testing of validate_ut8()..." );
  test_validate_ut8();
  logger_i("test_validate_ut8", "test successful");
}