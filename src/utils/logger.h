#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

namespace fm {

class Logger {
 public:
  void Write(std::string_view info) {
    std::ofstream logFile{fs::path{logDirName}, std::ios::app};
    if (!logFile.is_open()) {
      std::cerr << "!logFile.is_open()" << std::endl;
      return;
    }

    logFile << info << "\n";
    std::cout << info << "\n";

    logFile.close();
  }

  static Logger Get() {
    static Logger logger;
    return logger;
  }

 private:
  Logger() = default;

  std::string logDirName = "log";
};

}  // namespace fm