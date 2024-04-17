#pragma once

#include <string>
#include <vector>
#include <postgresql/libpq-fe.h>
#include <cstdint>

namespace fm {

using response = std::pair<int32_t, std::string>;

struct Actor {
  std::string name;
  std::string gender;
  std::string date;
};

struct Film {
  std::string id;
  std::string title;
  std::string description;
  std::string rating;
  std::vector<std::string> ids;  // of actors
};

class EndPoints {
 public:
  EndPoints();
  ~EndPoints();
  response AddActor(const Actor& actor);
  std::string GetActors();
 private:
  PGconn* database;
};

}  // namespace fm
