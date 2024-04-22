#include "server/server.h"

int main(int, char**) {
  fm::Server server;
  server.Testing();

  return server.Start();
}