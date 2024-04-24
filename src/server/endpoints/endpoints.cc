#include "endpoints.h"

#include "iostream"
#include "json.hpp"
#include "../../utils/utils.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

namespace fm {

EndPointsHandler::EndPointsHandler() : database(Utils::GetDatabaseConfig(fs::path{".local/database.conf"})) { 
  prepare(database, "add_actor", "INSERT INTO actor (actorname, gender, birthday) VALUES ($1, $2, $3)", 3, nullptr);
  prepare(database, "add_film", "INSERT INTO film (title, description, release_date, rating, actors) VALUES ($1, $2, $3, $4, $5)", 5, nullptr);
}

response EndPointsHandler::AddActor(const Actor &actor) {
  json response;
  response["message"] = "Successfully adding a new user " + actor.name;
  const char *paramValues[3] = {actor.name.c_str(), actor.gender.c_str(), actor.date.c_str()};

  execPrepared(database, "add_actor", 3, paramValues);

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

  execPrepared(database, "add_film", 5, paramValues);

  return {200, response.dump(4)};
}

response EndPointsHandler::GetFilms() {
  json response;

  PGresult* res = executeQuery(database, "SELECT f.id, f.title, f.description, f.release_date, f.rating, array_agg(a.actorname) AS actors FROM film f LEFT JOIN actor a ON a.id = ANY(f.actors) GROUP BY f.id;");

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
