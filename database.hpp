#include <iostream>
#include <pqxx/pqxx>

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
    void createTableSymbol();
    void createTableAccount();
    void createTableOrder();
    void executeSql(string sql);
};