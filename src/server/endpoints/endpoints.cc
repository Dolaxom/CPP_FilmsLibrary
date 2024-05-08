#include "endpoints.h"

#include "iostream"
#include "json.hpp"
#include "../../utils/utils.h"

using json = nlohmann::json;
namespace fs = std::filesystem;

#define JSON_DUMP -1

namespace fm {

EndPointsHandler::EndPointsHandler() : database{Utils::GetDatabaseConfig(fs::path{".local/database.conf"})}, redis{"127.0.0.1", 6379} { 
  prepare(database, "add_actor", "INSERT INTO actor (actorname, gender, birthday) VALUES ($1, $2, $3)", 3, nullptr);
  prepare(database, "add_film", "INSERT INTO film (title, description, release_date, rating, actors) VALUES ($1, $2, $3, $4, $5)", 5, nullptr);
}

response EndPointsHandler::AddActor(const Actor &actor) {
  json response;
  response["message"] = "Successfully adding a new user " + actor.name;
  constexpr int32_t paramsCount = 3;
  const char *paramValues[paramsCount] = {actor.name.c_str(), actor.gender.c_str(), actor.date.c_str()};

  execPrepared(database, "add_actor", paramsCount, paramValues);

  return {200, response.dump(JSON_DUMP)};
}

response EndPointsHandler::GetActors() {
  if (auto content = get(redis, "actors"); content.first == REDIS_REPLY_STRING) {
    return {200, content.second};
  } else {
    json response = get(database, "SELECT * FROM actor");
    std::string responseStr =  response.dump(JSON_DUMP);
    set(redis, "actors", responseStr);
    return {200, responseStr};  
  }
  return {500, "Redis/Postgres error."};
}

response EndPointsHandler::AddFilm(const Film& film) {
  json response;
  response["message"] = "Successfully adding a new film " + film.title;
  constexpr int32_t paramsCount = 5;
  const char *paramValues[paramsCount] = {film.title.c_str(), film.description.c_str(), film.release_date.c_str(), film.rating.c_str(), film.ids.c_str()};

  execPrepared(database, "add_film", paramsCount, paramValues);

  return {200, response.dump(JSON_DUMP)};
}

response EndPointsHandler::GetFilms() {
  if (auto content = get(redis, "films"); content.first == REDIS_REPLY_STRING) {
    return {200, content.second};
  } else {
    json response = get(database, "SELECT f.id, f.title, f.description, f.release_date, f.rating, array_agg(a.actorname) AS actors FROM film f LEFT JOIN actor a ON a.id = ANY(f.actors) GROUP BY f.id;");
    std::string responseStr =  response.dump(JSON_DUMP);
    set(redis, "films", responseStr);
    return {200, responseStr};  
  }

  return {500, "Redis/Postgres error."};
}

}  // namespace fm
