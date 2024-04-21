#pragma once

#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

namespace fm {

class Utils {
 public:
  static std::string ParseDatabaseConfig() {
    std::ifstream config{std::filesystem::path{"src/data/database.conf"}};
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
};

}  // namespace fm