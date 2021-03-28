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
  string msg=db.createAccount(0,1000);
  cout<<msg<<"create account id 0, balance 1000"<<endl;
  db.createAccount(1,3000);
  cout<<"create account id 1, balance 1000"<<endl;
  string msg1=db.createSymbol("abc",0,100);
  cout<<msg1<<"create abc 100 for id 0"<<endl;

  db.createOrder("abc",-10,20,1);
  db.createOrder("abc",-30,30,1);
  db.createOrder("abc",-20,15,1);
  db.createOrder("abc",40,20,0);
  
 
  
  db.disconnect();

  return 0;
  
}
