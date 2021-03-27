#include <iostream>
#include <pqxx/pqxx>
#include <sstream>
#include "MyException.hpp"

using namespace std;
using namespace pqxx;


class Database{
  private:
    connection *C;
  public:
    Database(){
      C=NULL;
    }
    void openDatabase();
    void dropTable();
    void createTableSymbol();
    void createTableAccount();
    void createTableOrder();
    void createTables();
    void executeSql(string sql);
    void disconnect();
    void createType();
    void createStatus();
    string createAccount(int id, double balance);
    string createSymbol(string name, int owner, int amount);
};