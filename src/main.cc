#include "crow.h"
#include "server/endpoints.h"
#include "json.hpp"

using json = nlohmann::json;

int main(int, char**) {
  crow::SimpleApp app;
  fm::EndPoints endpoints;

  CROW_ROUTE(app, "/actors").methods(crow::HTTPMethod::Get)([&]() {
    auto [code, body] = endpoints.GetActors();

    crow::response response(code, body);
    response.set_header("Content-Type", "application/json");
    return response;
  });

  CROW_ROUTE(app, "/actors").methods(crow::HTTPMethod::Post)([&](const crow::request& request) {
    json req = json::parse(request.body);

    auto [code, body] = endpoints.AddActor(fm::Actor{req["name"], req["gender"], req["date"]});

    crow::response response(code, body);
    response.set_header("Content-Type", "application/json");
    return response;
  });

  app.port(18080).multithreaded().run();
}