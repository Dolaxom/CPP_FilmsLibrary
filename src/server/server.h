#pragma once

#include <cstdint>

#include "crow.h"
#include "endpoints/endpoints.h"

namespace fm {

class Server {
 public:
  void Testing();
  int32_t Start();

 private:
  EndPointsHandler endpointsHandler;
  crow::SimpleApp app;

 private:
  // endpoints
  void Actors();
  void Films();
};

}  // namespace fm