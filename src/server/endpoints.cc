#include "endpoints.h"

#include "iostream"
#include "json.hpp"
#include "../utils/utils.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

namespace fm {

EndPoints::EndPoints() :
  database(Utils::GetDatabaseConfig(fs::path{".local/database.conf"})) { }

response EndPoints::AddActor(const Actor &actor) {
  json response;
  response["message"] = "Successfully adding a new user " + actor.name;
  const char *paramValues[3] = {actor.name.c_str(), actor.gender.c_str(), actor.date.c_str()};

  prepare(database, "insert_query", "INSERT INTO actor (actorname, gender, birthday) VALUES ($1, $2, $3)", 3, nullptr);
  execPrepared(database, "insert_query", 3, paramValues);

  return {200, response.dump(4)};
}

response EndPoints::GetActors() {
  json response;

  PGresult* res = executeQuery(database, "SELECT * FROM actor");

  int32_t rows = PQntuples(res);
  int32_t cols = PQnfields(res);
  for (int32_t i = 0; i < rows; ++i) {
    json part;
    for (int32_t j = 0; j < cols; ++j) {
      part[PQfname(res, j)] = PQgetvalue(res, i, j);
    }
    response += part;
  }
  PQclear(res);

  return {200, response.dump(4)};
}

}  // namespace fm
