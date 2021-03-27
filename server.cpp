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

  db.createTables();
  string msg=db.createSymbol("abc",0,100);
  cout<<msg<<endl;

  db.disconnect();

  return 0;
  
}
