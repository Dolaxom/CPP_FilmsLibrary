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
  std::string title;
  std::string description;
  std::string release_date;
  std::string rating;
  std::string ids;  // of actors
};

class EndPointsHandler {
 public:
  EndPointsHandler();
  response AddActor(const Actor& actor);
  response GetActors();
  response AddFilm(const Film& film);
  response GetFilms();
 private:
  PGConnection database;
};

}  // namespace fm
