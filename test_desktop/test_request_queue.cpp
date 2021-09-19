#include <stdexcept>
#include <cassert>
#include <ArduinoJson.h>

#include "../src/console_logger.h"
#include "../src/connectors/request_queue.h"
#include "../src/connectors/request.h"

void test_request_queue() {
  RequestQueue queue;

  assert(queue.isEmpty());

  auto test_req = std::make_shared<Request>("/test",
                                            12345,
                                            "testsuite",
                                            "nobody",
                                            DynamicJsonDocument(15));

  queue.push(test_req);

  assert(!queue.isEmpty());

  auto out_req = queue.pop();

  assert(out_req != nullptr);
  assert(*test_req == *out_req);
  assert(queue.isEmpty());
}

int main () {
  logger_i("RequestQueue", "starting test of request queue...");
  test_request_queue();
  logger_i("RequestQueue", "test successful");
}