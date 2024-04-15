#include "connector.h"

namespace fm {
PostgresConnection::PostgresConnection() : connection(nullptr) {}

PostgresConnection::~PostgresConnection() {
  if (connection != nullptr) {
    PQfinish(connection);
  }
}

bool PostgresConnection::connect(const char *conninfo) {
  connection = PQconnectdb(conninfo);
  if (PQstatus(connection) == CONNECTION_BAD) {
    std::cerr << "Connection to database failed: " << PQerrorMessage(connection)
              << std::endl;
    PQfinish(connection);
    connection = nullptr;
    return false;
  }
  return true;
}

PGresult *PostgresConnection::execQuery(const char *query) {
  if (connection == nullptr) {
    std::cerr << "Not connected to any database" << std::endl;
    return nullptr;
  }

  PGresult *result = PQexec(connection, query);
  if (PQresultStatus(result) == PGRES_FATAL_ERROR) {
    PQclear(result);
    throw std::runtime_error(PQerrorMessage(connection));
  }

  return result;
}

void PostgresConnection::close() {
  if (connection != nullptr) {
    PQfinish(connection);
    connection = nullptr;
  }
}

}  // namespace fm
