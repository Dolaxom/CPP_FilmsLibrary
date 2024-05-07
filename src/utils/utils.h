#pragma once

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include "json.hpp"
#include "server/endpoints/endpoints.h"

using json = nlohmann::json;

namespace fm {

class Utils {
 public:
  static std::string GetDatabaseConfig(const std::filesystem::path& path) {
    std::ifstream config{path};
    if (!config.is_open()) {
      throw std::runtime_error("Can't open database config file");
    }

    std::string result;

    std::string line;
    while (std::getline(config, line)) {
      result += line + " ";
    }

    return result;
  }

  static Actor JsonBodyToActor(std::string_view content) {
    json req = json::parse(content);
    json err;
    if (req.find("name") == req.end()) {
      err["description"] = "Can`t find name";
    } else if (req.find("gender") == req.end()) {
      err["description"] = "Can`t find gender";
    } else if (req.find("date") == req.end()) {
      err["description"] = "Can`t find date";
    }

    if (err.find("description") != err.end()) throw std::runtime_error(err.dump());

    return Actor{req["name"], req["gender"], req["date"]};
  }

  static Film JsonBodyToFilm(std::string_view content) {
    json req = json::parse(content);
    json err;
    if (req.find("title") == req.end()) {
      err["description"] = "Can`t find title";
    } else if (req.find("desription") == req.end()) {
      err["description"] = "Can`t find desription";
    } else if (req.find("date") == req.end()) {
      err["description"] = "Can`t find date";
    } else if (req.find("rating") == req.end()) {
      err["description"] = "Can`t find rating";
    } else if (req.find("actors") == req.end()) {
      err["description"] = "Can`t find actors";
    }

    if (err.find("description") != err.end()) throw std::runtime_error(err.dump());

    return Film{req["title"], req["desription"], req["date"], req["rating"], req["actors"]};
  }
};

}  // namespace fm