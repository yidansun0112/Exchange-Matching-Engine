#include "database.hpp"

void Database::openDatabase(){
  C = new connection("dbname=exchange user=postgres password=passw0rd");
  if (C->is_open()) {
    cout << "Opened database successfully: " << C->dbname() << endl;
  } else {
    cerr << "Can't open database" << endl;
    throw exception();
  }
}

void Database::dropTable(){
  string dropHeader="DROP TABLE IF EXISTS";
  string dropEnder="CASCADE;\n";
  string dropAccount=dropHeader+" ACCOUNT "+dropEnder;
  string dropSymbol=dropHeader+" SYMBOL "+dropEnder;
  string dropOrder=dropHeader+" ORDERS "+dropEnder;
  string dropTotal=dropAccount+dropSymbol+dropOrder+"\n";
  executeSql(dropTotal);
}

void Database::createTableSymbol(){
  string header="CREATE TABLE IF NOT EXISTS SYMBOL\n(";
  string ender=");\n";
  string name="NAME VARCHAR(50) NOT NULL,\n";
  string owner="OWNER INT NOT NULL,\n";
  string amount="AMOUNT FLOAT,\n";
  string primary_key="CONSTRAINT SYMPK PRIMARY KEY(NAME,OWNER),\n";
  string foreign_key="CONSTRAINT OWNERFK FOREIGN KEY(OWNER) REFERENCES ACCOUNT(ID) ON DELETE SET NULL ON UPDATE CASCADE\n";
  string total=header+name+owner+amount+primary_key+foreign_key+ender;
  executeSql(total);
}

void Database::createTableAccount(){
  string header="CREATE TABLE IF NOT EXISTS ACCOUNT\n(";
  string ender=");\n";
  string id="ID INT NOT NULL,\n";
  string balance="BALANCE FLOAT,\n";
  string primary_key="CONSTRAINT IDPK PRIMARY KEY(ID)\n";
  string total=header+id+balance+primary_key+ender;
  executeSql(total);
}

void Database::createTableOrder(){
  createType();
  createStatus();
  string header="CREATE TABLE IF NOT EXISTS ORDERS\n(";
  string ender=");\n";
  string id="ID SERIAL NOT NULL,\n";
  string symbol="SYM VARCHAR(50),\n";
  string amount="AMOUNT FLOAT,\n";
  string price="PRICE FLOAT,\n";
  string type="TYPE type,\n";
  string status="STATUS status,\n";
  string account_id="ACCOUNT_ID INT,\n";
  string trans_id="TRANS_ID INT,\n";
  string time="TIME TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,\n";
  string primary_key="CONSTRAINT ORDERPK PRIMARY KEY(ID),\n";
  string foreign_key="CONSTRAINT ACCOUNTFK FOREIGN KEY(ACCOUNT_ID) REFERENCES ACCOUNT(ID) ON DELETE SET NULL ON UPDATE CASCADE\n";
  string total=header+id+symbol+amount+price+type+status+account_id+trans_id+time+primary_key+foreign_key+ender;
  executeSql(total);
}

void Database::executeSql(string sql){
  work W(*C);
  W.exec(sql);
  W.commit();
}

void Database::createTables(){
  dropTable();
  createTableAccount();
  createTableSymbol();
  createTableOrder();
}

void Database::disconnect(){
  C->disconnect();
}

void Database::createType(){
 string begin="DO $$ BEGIN\n";
 string ifnot="IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname='type') THEN\n";
 string create="CREATE TYPE type AS ENUM('sell','buy');\n";
 string endif="END IF;\n";
 string end="END $$;\n";
 string total=begin+ifnot+create+endif+end;
 executeSql(total);
}

void Database::createStatus(){
 string begin="DO $$ BEGIN\n";
 string ifnot="IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname='status') THEN\n";
 string create="CREATE TYPE status AS ENUM('open','canceled','executed');\n";
 string endif="END IF;\n";
 string end="END $$;\n";
 string total=begin+ifnot+create+endif+end;
 executeSql(total);
}

string Database::createAccount(int id, double balance){
  if(balance<0){
    return "Account balance cannot be negative";
  }
  stringstream ss;
  ss<<"INSERT INTO ACCOUNT VALUES("<<id<<","<<balance<<");\n";
  string command=ss.str();
  try{
    executeSql(command);
  }catch(pqxx::unique_violation &e){
    return "This id already exists";
  }
  return "success";
}

string Database::createSymbol(string name, int owner, int amount){
  if(amount<0){
    return "Symbol amount cannot be negative";
  }
  stringstream ss;
  work W(*C);
  ss<<"INSERT INTO SYMBOL VALUES("<<W.quote(name)<<","<<owner<<","<<amount<<") ";
  ss<<"ON CONFLICT ON CONSTRAINT SYMPK DO UPDATE ";
  ss<<"SET AMOUNT=SYMBOL.AMOUNT+"<<amount<<";\n";
  W.commit();
  string command=ss.str();
  try{
    executeSql(command);
  }catch(pqxx::pqxx_exception &e){
    return "This id does not exist in Account";
  }
  return "success";
}