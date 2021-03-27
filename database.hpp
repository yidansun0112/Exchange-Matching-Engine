#include <iostream>
#include <pqxx/pqxx>
#include <sstream>
#include "MyException.hpp"

using namespace std;
using namespace pqxx;


class Database{
  private:
    connection *C;
    int trans_id;
  public:
    Database(){
      C=NULL;
      trans_id=0;
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
    string createOrder(string name,double amount,double price, int account_id);
    void minusSellAmount(string name,double amount,int account_id);
    void minusBuyBalance(double amount, double price,int account_id);
};