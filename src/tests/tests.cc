#include "tests.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "utils/utils.h"
#ifdef __APPLE__
#include "libpq-fe.h"
#else
#include <postgresql/libpq-fe.h>
#endif

namespace fs = std::filesystem;

namespace fm {

TEST(Database, 1) {
  PGconn* database = PQconnectdb(Utils::GetDatabaseConfig(fs::path{".local/database.conf"}).c_str());
  bool res = PQstatus(database) == CONNECTION_OK;
  PQfinish(database);
  EXPECT_EQ(res, true);
}

bool runGTests() {
  ::testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}

}  // namespace fm