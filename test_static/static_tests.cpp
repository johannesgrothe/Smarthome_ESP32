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

  soft_assert(false);

  return true;
}

bool execute_test(std::string test_name, std::function<bool()> test_method) {
  bool test_result = false;
  try {
    bool test_result = test_method();
  } catch (...) {}

  if (test_result) {
//    logger.printfln(LOG_TYPE::INFO, "'%s' test passed", test_name.c_str());
//    logger << "'" << test_name << "' test passed" << std::endl;
  } else {
//    logger.printfln(LOG_TYPE::ERR, "'%s' test failed to pass", test_name.c_str());
//    logger.setLevel(LOG_TYPE::ERR) << "'" << test_name << "' test passed" << std::endl;
  }

  return test_result;
}

int main() {
  logger << "Starting Tests" << "\n";

  bool all_passed = true;

  all_passed &= execute_test("RequestQueue", test_request_queue);

  if (all_passed) {
//    logger << "All tests passed" << std::endl;
    return 0;
  }

//  logger << "Finished with failed tests" << std::endl;
  return 1;
}