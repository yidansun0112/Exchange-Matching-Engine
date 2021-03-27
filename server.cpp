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
  string msg=db.createAccount(0,-100);
  cout<<msg<<endl;
  db.createAccount(0,100);
  string msg1=db.createSymbol("abc",0,100);
  cout<<msg1<<endl;
  string msg2=db.createSymbol("abc",0,-100);
  cout<<msg2<<endl;

  string msg3=db.createOrder("abc",-50,100,0);
  cout<<msg3<<endl;

  string msg4=db.createOrder("abc",50,100,0);
  cout<<"id 5 "<<msg4<<endl;

  
  vector<string> v=db.cancelOrder(0);
  for(vector<string>::iterator i=v.begin();i!=v.end();++i){
    cout<<*i<<endl;
  }
  
  db.disconnect();

  return 0;
  
}
