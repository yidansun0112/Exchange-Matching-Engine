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
  db.createAccount(1,1000);
  cout<<"create account id 1, balance 1000"<<endl;
  string msg1=db.createSymbol("abc",0,100);
  cout<<msg1<<"create abc 100 for id 0"<<endl;


   string msg4=db.createOrder("abc",30,15,1);
  cout<<"id 1 buy 30 abc at $15"<<endl;

  
  string msg3=db.createOrder("abc",-50,10,0);
  cout<<"id 0 sell 50 abc at $10"<<endl;
  
  vector<string> v=db.cancelOrder(1);
  for(vector<string>::iterator i=v.begin();i!=v.end();++i){
    cout<<*i<<endl;
  }
  
  db.disconnect();

  return 0;
  
}
