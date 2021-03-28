#include <iostream>
#include <pqxx/pqxx>
#include <sstream>
#include <vector>
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
    vector<string> queryOrder(int trans_id);
    vector<string> cancelOrder(int trans_id);
    void matchSellOrder(string name, double amount, double price, int account_id,int trans_id);
    void matchBuyOrder(string name, double amount, double price, int account_id,int trans_id);
    void addNewLine(int trans_id, double price,double amount,string name,int account_id,string type);
    void changeStatus(int trans_id, double price);
    long getCurrTime();
    void addBuyAmount(string name,int owner, double amount);
    void addSellBalance(double amount, double price, int account_id);
    void editBuyBalance(double buy_price, double execPrice, double amount, int account_id);
};
