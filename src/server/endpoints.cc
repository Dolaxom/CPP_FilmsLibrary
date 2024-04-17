#include "endpoints.h"

#include "iostream"
#include "json.hpp"

using json = nlohmann::json;

namespace fm {

EndPoints::EndPoints() {
  database = PQconnectdb("host=localhost port=5433 dbname=filmoteka user=postgres password=admin");

  if (PQstatus(database) != CONNECTION_OK) {
    PQfinish(database);
    throw std::runtime_error("database connection error");
  }
}

EndPoints::~EndPoints() {
  PQfinish(database);
}

response EndPoints::AddActor(const Actor &actor) {
  json response;
  response["message"] = "Successfully adding a new user " + actor.name;

  PGresult *res = PQprepare(
      database, "insert_query",
      "INSERT INTO actor (actorname, gender, birthday) VALUES ($1, $2, $3)", 3,
      nullptr);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    PQclear(res);
    return {500, PQerrorMessage(database)};
  }
  PQclear(res);

  const char *paramValues[3] = {actor.name.c_str(), actor.gender.c_str(),
                                actor.date.c_str()};
  res = PQexecPrepared(database, "insert_query", 3, paramValues, NULL, NULL, 0);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    PQclear(res);
    return {500, PQerrorMessage(database)};
  }
  PQclear(res);

  return {200, response.dump(4)};
}

response EndPoints::GetActors() {
  json response;
  PGresult *res = PQexec(database, "SELECT * FROM actor");
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    PQclear(res);
    return {500, PQerrorMessage(database)};
  }

  int rows = PQntuples(res);
  int cols = PQnfields(res);
  for (int i = 0; i < rows; ++i) {
    json part;
    for (int j = 0; j < cols; ++j) {
      part[PQfname(res, j)] = PQgetvalue(res, i, j);
    }
    response += part;
  }
  PQclear(res);

  return {200, response.dump(4)};
}

}  // namespace fm
