#include "endpoints.h"

#include "iostream"
#include "json.hpp"
#include "../../utils/utils.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

namespace fm {

EndPointsHandler::EndPointsHandler() :
  database(Utils::GetDatabaseConfig(fs::path{".local/database.conf"})) { }

response EndPointsHandler::AddActor(const Actor &actor) {
  json response;
  response["message"] = "Successfully adding a new user " + actor.name;
  const char *paramValues[3] = {actor.name.c_str(), actor.gender.c_str(), actor.date.c_str()};

  prepare(database, "insert_query", "INSERT INTO actor (actorname, gender, birthday) VALUES ($1, $2, $3)", 3, nullptr);
  execPrepared(database, "insert_query", 3, paramValues);

  return {200, response.dump(4)};
}

response EndPointsHandler::GetActors() {
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

response EndPointsHandler::AddFilm(const Film& film) {
  json response;
  response["message"] = "Successfully adding a new user " + film.title;
  const char *paramValues[5] = {film.title.c_str(), film.description.c_str(), film.release_date.c_str(), film.rating.c_str(), film.ids.c_str()};

  prepare(database, "insert_query", "INSERT INTO film (title, description, release_date, rating, actors) VALUES ($1, $2, $3, $4, $5)", 5, nullptr);
  execPrepared(database, "insert_query", 5, paramValues);

  return {200, response.dump(4)};
}

response EndPointsHandler::GetFilms() {
  json response;

  PGresult* res = executeQuery(database, "SELECT * FROM film");

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
