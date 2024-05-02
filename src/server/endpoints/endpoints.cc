#include "endpoints.h"

#include "iostream"
#include "json.hpp"
#include "../../utils/utils.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

#define JSON_DUMP -1

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

  return {200, response.dump(JSON_DUMP)};
}

response EndPointsHandler::GetActors() {
  json response = get(database, "SELECT * FROM actor");

  return {200, response.dump(JSON_DUMP)};
}

response EndPointsHandler::AddFilm(const Film& film) {
  json response;
  response["message"] = "Successfully adding a new film " + film.title;
  const char *paramValues[5] = {film.title.c_str(), film.description.c_str(), film.release_date.c_str(), film.rating.c_str(), film.ids.c_str()};

  execPrepared(database, "add_film", 5, paramValues);

  return {200, response.dump(JSON_DUMP)};
}

response EndPointsHandler::GetFilms() {
  json response = get(database, "SELECT f.id, f.title, f.description, f.release_date, f.rating, array_agg(a.actorname) AS actors FROM film f LEFT JOIN actor a ON a.id = ANY(f.actors) GROUP BY f.id;");

  return {200, response.dump(JSON_DUMP)};
}

}  // namespace fm
