#include "catch.hpp"

#include "../src/helpers/threadsafe_queue.h"
#include <vector>
#include <string>
#include <memory>

TEST_CASE("Test Threadsafe Queue", "[Helpers]") {
  ThreadSafeQueue<std::shared_ptr<std::string>> queue;
  SECTION("Test Empty") {
    CHECK(queue.isEmpty());
    CHECK(queue.pop() == nullptr);
  }
  SECTION("Test with elements") {
    auto ptr = std::make_shared<std::string>("yolokopter");
    queue.push(ptr);
    CHECK(!queue.isEmpty());
    CHECK(*queue.pop() == "yolokopter");
  }
}
