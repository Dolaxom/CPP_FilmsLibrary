#pragma once

#include <postgresql/libpq-fe.h>

#include <iostream>

namespace fm {

class PostgresConnection {
 public:
  PostgresConnection();

  ~PostgresConnection();

  bool connect(const char *conninfo);

  PGresult *execQuery(const char *query);

  void close();

 private:
  PGconn *connection;
};

}  // namespace fm