#include <cstdio>
#include "xmlParser.h"
#include <pqxx/pqxx>
#include "server.h"
using namespace pqxx;

int main() {
  Server server;
  server.run();
  return 0;
}
