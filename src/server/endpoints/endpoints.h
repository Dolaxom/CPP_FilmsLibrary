#pragma once

#include <string>
#include <vector>
#include "../../database/wrapper.h"
#include <cstdint>

namespace fm {

using response = std::pair<int32_t, std::string>; // <code, body>

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

class EndPointsHandler {
 public:
  EndPointsHandler();
  response AddActor(const Actor& actor);
  response GetActors();
 private:
  PGConnection database;
};

}  // namespace fm
