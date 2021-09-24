#include "catch.hpp"

#include <ArduinoJson.h>

#include "../src/console_logger.h"
#include "../src/connectors/request_queue.h"

TEST_CASE("Test Request Queue", "[RequestQueue]") {
  RequestQueue queue;

  CHECK(queue.isEmpty());

  auto test_req = std::make_shared<Request>("/test",
                                            12345,
                                            "testsuite",
                                            "nobody",
                                            DynamicJsonDocument(15));

  queue.push(test_req);

  CHECK(!queue.isEmpty());

  auto out_req = queue.pop();

  CHECK(out_req != nullptr);
  CHECK(*test_req == *out_req);
  CHECK(queue.isEmpty());
}
