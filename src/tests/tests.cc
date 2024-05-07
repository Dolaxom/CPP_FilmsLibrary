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

TEST(Util, 1) {
  auto actor = Utils::JsonBodyToActor(R"({ "name" : "a1", "gender" : "gen", "date" : "1010" })");
  EXPECT_EQ(actor.name, "a1");
  EXPECT_EQ(actor.gender, "gen");
  EXPECT_EQ(actor.date, "1010");
}

TEST(Util, 2) {
  auto film = Utils::JsonBodyToFilm(R"({ "title" : "film", "desription" : "---", "date" : "1-2-3", "rating" : "10", "actors" : "{1, 3}" })");
  EXPECT_EQ(film.title, "film");
  EXPECT_EQ(film.description, "---");
  EXPECT_EQ(film.release_date, "1-2-3");
  EXPECT_EQ(film.rating, "10");
  EXPECT_EQ(film.ids, "{1, 3}");
}

bool runGTests() {
  ::testing::InitGoogleTest();
  return RUN_ALL_TESTS();
}

}  // namespace fm