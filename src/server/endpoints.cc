#include "endpoints.h"

#include <postgresql/libpq-fe.h>
#include "iostream"
#include "json.hpp"

using json = nlohmann::json;

namespace fm {

EndPoints::EndPoints() {}

response EndPoints::AddActor(const Actor &actor) {
  json response;
  response["message"] = "Successfully adding a new user " + actor.name;

  // Устанавливаем соединение с базой данных
  PGconn *conn = PQconnectdb("host=localhost port=5433 dbname=filmoteka user=postgres password=admin");

  // Проверяем успешность соединения
  if (PQstatus(conn) != CONNECTION_OK) {
    PQfinish(conn);
    throw std::runtime_error("bebra1");
  }

  // Подготавливаем запрос на вставку данных
  PGresult* res = PQprepare(
      conn, "insert_query",
      "INSERT INTO actor (actorname, gender, birthday) VALUES ($1, $2, $3)", 3,
      nullptr);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    std::cerr << "PREPARE failed: " << PQerrorMessage(conn) << std::endl;
    PQclear(res);
    PQfinish(conn);
    throw std::runtime_error("bebra2");
  }
  PQclear(res);

  // Вставляем данные в таблицу
  const char *paramValues[3] = {actor.name.c_str(), actor.gender.c_str(), actor.date.c_str()};
  res = PQexecPrepared(conn, "insert_query", 3, paramValues, NULL, NULL, 0);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    PQclear(res);
    PQfinish(conn);
    throw std::runtime_error("bebra3");
  }
  PQclear(res);

  // Закрываем соединение с базой данных
  PQfinish(conn);

  return {200, response.dump(4)};
}

}  // namespace fm
