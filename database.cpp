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
  string foreign_key="CONSTRAINT OWNERFK FOREIGN KEY(OWNER) REFERENCES ACCOUNT(ID) ON DELETE SET NULL ON UPDATE CASCADE,\n";
  string amount_nonneg="CONSTRAINT amountnonneg CHECK (AMOUNT>=0)\n";
  string total=header+name+owner+amount+primary_key+foreign_key+amount_nonneg+ender;
  executeSql(total);
}

void Database::createTableAccount(){
  string header="CREATE TABLE IF NOT EXISTS ACCOUNT\n(";
  string ender=");\n";
  string id="ID INT NOT NULL,\n";
  string balance="BALANCE FLOAT,\n";
  string primary_key="CONSTRAINT IDPK PRIMARY KEY(ID),\n";
  string baln_nonneg="CONSTRAINT balnnonneg CHECK (BALANCE>=0)\n";
  string total=header+id+balance+primary_key+baln_nonneg+ender;
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
  string time="TIME BIGINT NOT NULL,\n";
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
  stringstream ss;
  ss<<"INSERT INTO ACCOUNT VALUES("<<id<<","<<balance<<");\n";
  string command=ss.str();
  try{
    executeSql(command);
  }catch(pqxx::unique_violation &e){
    return "This id already exists";
  }catch(pqxx::check_violation &e){
    return "Account balance cannot be negative";
  }
  return "success";
}

string Database::createSymbol(string name, int owner, int amount){
  stringstream ss;
  work W(*C);
  ss<<"INSERT INTO SYMBOL VALUES("<<W.quote(name)<<","<<owner<<","<<amount<<") ";
  ss<<"ON CONFLICT ON CONSTRAINT SYMPK DO UPDATE ";
  ss<<"SET AMOUNT=SYMBOL.AMOUNT+"<<amount<<";\n";
  W.commit();
  string command=ss.str();
  try{
    executeSql(command);
  }catch(pqxx::foreign_key_violation &e){
    return "This id does not exist in Account";
  }catch(pqxx::check_violation &e){
    return "Symbol amount cannot be negative";
  }
  return "success";
}

string Database::createOrder(string name,double amount,double price, int account_id){
  stringstream ss;
  work W(*C);
  ss<<"INSERT INTO ORDERS (SYM,AMOUNT,PRICE,TYPE,STATUS,ACCOUNT_ID,TRANS_ID,TIME) VALUES(";
  double amt=amount>0?amount:-amount;
  ss<<W.quote(name)<<","<<amt<<","<<price<<",";
  W.commit();
  try{
    if(amount<0){
      ss<<"'sell'"<<",";
      amount=-amount;
      minusSellAmount(name,amount,account_id);
    }else if(amount>0){
      ss<<"'buy'"<<",";
      minusBuyBalance(amount,price,account_id);
    }else{
      return "Order amount cannot be zero";
    }
    ss<<"'open'"<<","<<account_id<<","<<trans_id<<","<<getCurrTime()<<");\n";
    string command=ss.str();
    cout<<command;
    executeSql(command);
  }catch(pqxx::foreign_key_violation &e){
    return "This id does not exist in Account";
  }catch(pqxx::check_violation &e){
    if(amount<0){
      return "You don't have enough amount to sell";
    }else{
      return "You don't have enough balance to buy";
    }
  }catch (MyException &e){
    return e.what();
  }
  stringstream sss;
  sss<<"success id is "<<trans_id;
  trans_id++;
  return sss.str();
}


void Database::minusSellAmount(string name,double amount,int account_id){
  stringstream ss;
  work W(*C);
  ss<<"UPDATE SYMBOL SET AMOUNT=SYMBOL.AMOUNT-"<<amount<<" WHERE SYMBOL.NAME="<<W.quote(name);
  ss<<" AND OWNER="<<account_id<<";\n";
  W.commit();
  string command=ss.str();
  string select="SELECT * FROM SYMBOL "+command.substr(command.find("WHERE"));
  work w(*C);
  result r=w.exec(select);
  w.commit();
  if(r.size()==0){
    throw MyException("Cannot find this symbol");
  }
  executeSql(command);
}

void Database::minusBuyBalance(double amount, double price,int account_id){
  stringstream ss;
  double cost=price*amount;
  ss<<"UPDATE ACCOUNT SET BALANCE=BALANCE-"<<cost<<" WHERE ID="<<account_id<<";\n";
  cout<<ss.str();
  executeSql(ss.str());
}

vector<string> Database::queryOrder(int trans_id){
  stringstream ss;
  vector<string> v;
  ss<<"SELECT * FROM ORDERS WHERE TRANS_ID="<<trans_id<<";\n";
  work W(*C);
  result r=W.exec(ss.str());
  W.commit();
  int num_rows=r.size();
  cout<<num_rows<<endl;
  for(int i=0;i<num_rows;i++){
    pqxx::row const row = r[i];
    if(string(row[5].c_str())=="open"){
        stringstream ss;
        ss<<"open shares="<<row[2].c_str();
        v.push_back(ss.str());
      }
    if(string(row[5].c_str())=="canceled"){
        stringstream ss;
        ss<<"canceled shares="<<row[2].c_str()<<" time="<<row[8];
        v.push_back(ss.str());
      }
    if(string(row[5].c_str())=="executed"){
        stringstream ss;
        ss<<"executed shares="<<row[2].c_str()<<" price="<<row[3].c_str()<<" time="<<row[8];
        v.push_back(ss.str());
      }
  }
  return v;
}

vector<string> Database::cancelOrder(int trans_id){
  stringstream ss;
  ss<<"UPDATE ORDERS SET STATUS='canceled', TIME="<<getCurrTime()<<" WHERE TRANS_ID="<<trans_id<<" AND STATUS='open';\n";
  executeSql(ss.str());
  return queryOrder(trans_id);
}

void Database::matchSellOrder(string name, double amount, double price, int account_id, long time, int trans_id){
  stringstream ss;
  ss<<"SELECT * FROM ORDERS WHERE STATUS='open' AND TYPE='buy' AND PRICE>="<<price<<" AND SYM="<<"'"<<name<<"'";
  ss<<"ORDER BY PRICE DESC, TIME;\n";
  work W(*C);
  result r=W.exec(ss.str());
  int num_row=r.size();
  int i=0;
  while(amount>0&&i<num_row){
    pqxx::row const row=r[i];
    int buy_transId=row[7].as<int>();
    int buy_accountId=row[6].as<int>();
    double buy_amount=row[2].as<double>();
    double buy_price=row[3].as<double>();
    long buy_time=row[8].as<long>();
    double execPrice=price;
    if(buy_time<time){
      execPrice=buy_price;
    }
    if(amount==buy_amount){
      changeStatus(trans_id,execPrice);
      changeStatus(buy_transId,execPrice);
      amount=0;
    }else if(amount<buy_amount){
      changeStatus(trans_id,execPrice);
      addNewLine(buy_transId, execPrice,amount,name, buy_accountId,"'buy'");
      amount=0;
    }else{
      changeStatus(buy_transId,execPrice);
      addNewLine(trans_id, execPrice,buy_amount,name,account_id,"'sell'");
      amount-=buy_amount;
    }
    i++;
  }
}


void Database::matchBuyOrder(string name, double amount, double price, int account_id, long time, int trans_id){

}


void Database::addNewLine(int trans_id, double price,double amount,string name, int account_id, string type){
  stringstream s1;
  s1<<"UPDATE ORDERS SET AMOUNT=ORDERS.AMOUNT-"<<amount<<" WHERE TRANS_ID="<<trans_id<<" AND STATUS='open';\n";
  executeSql(s1.str());
  stringstream s2;
  s2<<"INSERT INTO ORDERS (SYM,AMOUNT,PRICE,TYPE,STATUS,ACCOUNT_ID,TRANS_ID,TIME) VALUES(";
  s2<<"'"<<name<<"',"<<amount<<","<<price<<","<<type<<","<<"'executed',"<<account_id<<","<<trans_id<<","<<getCurrTime()<<";\n";
  executeSql(s2.str());
}

void Database::changeStatus(int trans_id, double price){
  stringstream ss;
  ss<<"UPDATE ORDERS SET STATUS='executed', TIME="<<getCurrTime()<<", PRICE="<<price<<" WHERE TRANS_ID="<<trans_id<<" AND STATUS='open';\n";
  executeSql(ss.str());
}

long Database::getCurrTime(){
  time_t now=time(NULL);
  return (long)now;
}
