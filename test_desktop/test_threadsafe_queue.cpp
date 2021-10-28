#include "catch.hpp"

#include "../src/helpers/threadsafe_queue.h"
#include <string>
#include <memory>

TEST_CASE("Test Threadsafe Queue", "[Helpers]") {
  ThreadSafeQueue<std::shared_ptr<std::string>> queue;

  SECTION("Test Empty") {
    // Queue is empty after initialization
    CHECK(queue.isEmpty());
    CHECK(queue.pop() == nullptr);
  }

  SECTION("Test with elements") {
    auto ptr = std::make_shared<std::string>("yolokopter");
    queue.push(ptr);

    // Queue is not empty after push
    CHECK(!queue.isEmpty());
    CHECK(*queue.pop() == "yolokopter");

    // Queue is empty after pop
    CHECK(queue.isEmpty());
    CHECK(queue.pop() == nullptr);
  }
}
