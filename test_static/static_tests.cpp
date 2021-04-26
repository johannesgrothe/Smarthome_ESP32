#include <stdexcept>

#include "../src/console_logger.h"
#include "../src/connectors/request_queue.cpp"

static void soft_assert(bool assert_value) {
  if (!assert_value) {
    throw std::exception();
  }
}

bool test_request_queue() {
  RequestQueue queue;

  soft_assert(queue.isEmpty());

  auto test_req = std::make_shared<Request>("/test",
                                            12345,
                                            "testsuite",
                                            "nobody",
                                            DynamicJsonDocument(15));

  queue.push(test_req);
  soft_assert(!queue.isEmpty());

  auto out_req = queue.pop();

  soft_assert(out_req != nullptr);

  soft_assert(*test_req == *out_req);

  return true;
}

bool execute_test(std::string test_name, std::function<bool()> test_method) {
  bool test_result = false;
  try {
    test_result = test_method();
  } catch (...) { }

  if (test_result) {
    logger.setSender("Testsuite") << "Test '" << test_name << "' passed\n";
  } else {
    logger.setSender("Testsuite").setLevel(LOG_TYPE::ERR) << "'" << test_name << "' test failed\n";
  }

  return test_result;
}

int main() {
  logger.setSender("Testsuite") << "Starting Tests" << "\n";

  ++ logger;

  bool all_passed = true;

  all_passed &= execute_test("RequestQueue", test_request_queue);

  -- logger;

  if (all_passed) {
    logger.setSender("Testsuite") << "All tests passed" << "\n";
    return 0;
  }

  logger.setSender("Testsuite") << "Finished with failed tests" << "\n";
  return 1;
}