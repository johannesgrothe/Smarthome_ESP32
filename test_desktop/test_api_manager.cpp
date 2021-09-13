#include <stdexcept>
#include <cassert>

#include "../src/api_manager.h"
#include "../src/test_dummys/api_manager_delegate_dummy.h"

#define TEST_CLIENT_ID "test_client"

// Test function
int main () {
  auto delegate = ApiManagerDelegateDummy(TEST_CLIENT_ID);
  auto manager = ApiManager(&delegate);
}
