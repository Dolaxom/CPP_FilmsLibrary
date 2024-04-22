#include "utils/logger.h"
#include "server.h"
#include "endpoints/endpoints.h"
#include "json.hpp"

using json = nlohmann::json;

namespace fm {

void Server::Testing() {
  // TODO
  // google tests or Catch2
}

int32_t Server::Start() {
  try {
    Actors();
    Films();

    app.port(18080).multithreaded().run();
  } catch (const std::exception& ex) {
    Logger::Get().Write(ex.what());
    return 1;
  } catch (...) {
    return 2;
  }

  return 0;
}

void Server::Actors() {
  CROW_ROUTE(app, "/actors").methods(crow::HTTPMethod::Get)([&]() {
    auto [code, body] = endpointsHandler.GetActors();

    crow::response response(code, body);
    response.set_header("Content-Type", "application/json");
    return response;
  });

  CROW_ROUTE(app, "/actors").methods(crow::HTTPMethod::Post)([&](const crow::request& request) {
        json req = json::parse(request.body);

        Actor actor{req["name"], req["gender"], req["date"]};
        auto [code, body] = endpointsHandler.AddActor(actor);

        crow::response response(code, body);
        response.set_header("Content-Type", "application/json");
        return response;
      });
}
void Server::Films(){/* TODO */};

}  // namespace fm