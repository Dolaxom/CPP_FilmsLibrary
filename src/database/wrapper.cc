#include "wrapper.h"

#include <stdexcept>

namespace fm {

PGConnection::PGConnection(const std::string& params) {
  database = PQconnectdb(params.c_str());

  if (PQstatus(database) != CONNECTION_OK) {
    PQfinish(database);
    throw std::runtime_error("Postgres connection status != OK");
  }
}

PGConnection::~PGConnection() { PQfinish(database); }

PGConnection::PGConnection(PGConnection&& other) {
  database = other.database;
  other.database = nullptr;
}

void prepare(PGConnection& database, const std::string& type, const std::string& query, int32_t count, const Oid* paramTypes) {
  PGresult* res = PQprepare(database.database, type.c_str(), query.c_str(), count, paramTypes);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    PQclear(res);
    throw std::runtime_error(std::string("prepare(): ") + PQerrorMessage(database.database));
  }
  PQclear(res);
}

void execPrepared(PGConnection& database, const std::string& type, int32_t count, const char* const* paramValues) {
  PGresult* res = PQexecPrepared(database.database, type.c_str(), count, paramValues, nullptr, nullptr, 0);
  if (PQresultStatus(res) != PGRES_COMMAND_OK) {
    PQclear(res);
    throw std::runtime_error(std::string("execPrepared(): ") + PQerrorMessage(database.database));
  }
  PQclear(res);
}

PGresult* executeQuery(PGConnection& database, const std::string& query) {
  PGresult* res = PQexec(database.database, query.c_str());
  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    PQclear(res);
    throw std::runtime_error(PQerrorMessage(database.database));
  }

  return res;
}


}  // namespace fm
