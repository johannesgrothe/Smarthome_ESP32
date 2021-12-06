
#include "catch.hpp"

#include "../src/api/client_meta.h"


TEST_CASE("Test Client Meta", "[Container]") {
  ClientMeta meta({},
                  BootMode::Full_Operation,
                  "",
                  "commit_hash",
                  "develop");

  SECTION("Test Constructor") {
    CHECK(meta.port_mapping.empty());
    CHECK(meta.boot_mode == BootMode::Full_Operation);
    CHECK(meta.sw_uploaded.empty());
    CHECK(meta.sw_commit == "commit_hash");
    CHECK(meta.sw_branch == "develop");
  }

}
