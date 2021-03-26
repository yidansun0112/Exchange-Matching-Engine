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
  string header="CREATE TABLE IF NOT EXISTS ORDER\n(";
  string ender=");\n";
  string id="ID SERIAL NOT NULL,\n";
  string symbol="SYM VARCHAR(50),\n";
  string amount="AMOUNT FLOAT,\n";
  string price="PRICE FLOAT,\n";
  string type="TYPE ENUM('sell','buy'),\n";
  string status="STATUS ENUM('open','canceled','executed'),\n";
  string account_id="ACCOUNT_ID INT,\n";
  string trans_id="TRANS_ID INT,\n";
  string time="TIME TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE,\n";
  string primary_key="CONSTRAINT IDPK PRIMARY KEY(ID)\n";
  string foreign_key="CONSTRAINT ACCOUNTFK FOREIGN KEY(ACCOUNT_ID) REFERENCES ACCOUNT(ID) ON DELETE SET NULL ON UPDATE CASCADE\n";
  string total=header+id+symbol+amount+price+type+status+account_id+trans_id+time+primary_key+foreign_key+ender;
  executeSql(total);
}

void Database::executeSql(string sql){
  work W(*C);
  W.exec(sql);
  W.commit();
}