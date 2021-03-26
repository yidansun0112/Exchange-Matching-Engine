#include <iostream>
#include <pqxx/pqxx>
#include "database.hpp"

using namespace std;
using namespace pqxx;

int main (int argc, char *argv[]) 
{

  //open database
  Database db;
  try{
    db.openDatabase();
  }catch (const exception &e){
    return 1;
  }

  

}
