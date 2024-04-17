#pragma once

#include <postgresql/libpq-fe.h>

#include <string>

namespace fm {

class PGConnection {
 public:
  PGConnection(const std::string& params);
  ~PGConnection();
  PGConnection(const PGConnection& other) = delete;
  PGConnection(PGConnection&& other);

 private:
  PGconn* database;

  // TODO maybe create class for every friend method. Чтобы в их деструкторе удалять PGresult в случае для executeQuery
  friend void prepare(PGConnection& database, const std::string& type,
                      const std::string& query, int32_t count,
                      const Oid* paramTypes);

  friend void execPrepared(PGConnection& database, const std::string& type,
                           int32_t count, const char* const* paramValues);

  friend PGresult* executeQuery(PGConnection& database, const std::string& query);
};

}  // namespace fm
