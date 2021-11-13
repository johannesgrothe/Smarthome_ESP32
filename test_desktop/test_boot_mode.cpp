#include "catch.hpp"

#include "../src/boot_mode.h"

TEST_CASE("Test Boot Mode", "") {
  SECTION("Test Boot Mode Serial Only") {
    HardwareController::lockPinValue(REG0, false);
    HardwareController::lockPinValue(REG1, false);
    HardwareController::lockPinValue(REG2, false);
    CHECK(getBootMode() == BootMode::Serial_Only);
  }

  SECTION("Test Boot Mode Network Only") {
    HardwareController::lockPinValue(REG0, false);
    HardwareController::lockPinValue(REG1, true);
    HardwareController::lockPinValue(REG2, false);
    CHECK(getBootMode() == BootMode::Network_Only);
  }

  SECTION("Test Boot Mode Full Operation") {
    HardwareController::lockPinValue(REG0, true);
    HardwareController::lockPinValue(REG1, true);
    HardwareController::lockPinValue(REG2, true);
    CHECK(getBootMode() == BootMode::Full_Operation);
  }
}
