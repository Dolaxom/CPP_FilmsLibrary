#include "server.h"
#include "utils/utils.h"
#include "utils/logger.h"
#include "tests/tests.h"

namespace fm {

void Server::Testing() {
  runGTests(); 
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

//! @brief /actors endpoint
//! GET/POST
void Server::Actors() {
  CROW_ROUTE(app, "/actors").methods(crow::HTTPMethod::Get)([&]() {
    auto [code, body] = endpointsHandler.GetActors();

    crow::response response(code, body);
    response.set_header("Content-Type", "application/json");
    return response;
  });

  CROW_ROUTE(app, "/actors").methods(crow::HTTPMethod::Post)([&](const crow::request& request) {\
    crow::response response;
    response.set_header("Content-Type", "application/json");

    try {
      auto actor = Utils::JsonBodyToActor(request.body);  
      auto [code, body] = endpointsHandler.AddActor(actor);
      response.code = code;
      response.body = body;
    } catch (const std::exception& ex) {
      response.code = 500;
      response.body = ex.what();
    }

    return response;
  });
}

//! @brief /films endpoint
//! GET/POST
void Server::Films() {
  CROW_ROUTE(app, "/films").methods(crow::HTTPMethod::Get)([&]() {
    auto [code, body] = endpointsHandler.GetFilms();

    crow::response response(code, body);
    response.set_header("Content-Type", "application/json");
    return response;
  });

  CROW_ROUTE(app, "/films").methods(crow::HTTPMethod::Post)([&](const crow::request& request) {
    crow::response response;
    response.set_header("Content-Type", "application/json");

    try {
      auto film = Utils::JsonBodyToFilm(request.body);  
      auto [code, body] = endpointsHandler.AddFilm(film);
      response.code = code;
      response.body = body;
    } catch (const std::exception& ex) {
      response.code = 500;
      response.body = ex.what();
    }

    return response;
  });
}

}  // namespace fm