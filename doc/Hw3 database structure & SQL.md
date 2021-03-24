# Hw3 database structure & SQL 

## Select * 多行怎么办

### table 1 Symbol

| Name    | Owner_id | amount           |
| ------- | -------- | ---------------- |
| VARCHAR | INT      | FLOAT (POSITIVE) |

Primary key : name + owner_id

Foreign key : owner_id --> Account account_id

### table 2 Account

| account_id | balance |
| ---------- | ------- |
| INT        | FLOAT   |

Primary key : account_id

### table 3 Order 

| Symbol  | amout | Price | type | Status | account_id | transaction_id | time       | Index |
| ------- | ----- | ----- | ---- | ------ | ---------- | -------------- | ---------- | ----- |
| VARCHAR | FLOAT | FLOAT | ENUM | ENUM   | INT        | INT            | Time Stamp | INT   |

Primary key : index

question : about & price : float / int

Type : sell / buy

Status: open, cancel, execute

Id : static variable, +1 when creating



### SQL 

#### create

##### create account:

INSERT INTO Account VALUES( )

check id not exist

check balance >=0

##### Create symbol

INSERT INTO Symbol VALUES ( )

check amount >=0

check amout_id exists



#### Transactions

##### order

INSERT INTO Order VALUES ( )

1. for buyer: check Account : balance > price * amout

2. for seller : check Symbol : amout > to_sell_amout

3. When creating,

   1. for buyer : balance -  price * amout
   2. for seller: amout - to_sell_amout

   

##### query

SELECT * from Order WHERE id=tras_id



##### Cancel

UPDATE Order SET Status=cancel Time=curr_time() WHERE id=tras_id and Status=open

update status & time

check status is open



### Match Order & Execute

aftering insert this order into Order, assume this order is to sell

Question : when multiple lines are returned, how to select one

SELECT * FROM Order WHERE status=open and type=buy and price>=sell_price and symbol=sell_symbol

from multiple line,

select highest price, if tie, select earliest 

Execute: 

​	for Order: 两者amount小方，改，status，price and time

​						amount 大方，新增一行executed，同时改原始行amount-=executed_amount

​	for buyer: change Symbol: add amount

​	for seller: change Account: add balance

After:

​	if sell order amount>0, continue



after inserting this order into Order, assume this order is to buy

SELECT * FROM Order WHERE status=open and type=sell and price<=buy_price and symbol=buy_symbol

from multiple line,

select lowest price, if tie, select earliest 

Execute: 

​	for Order:  两者amount小方，改，status，price and time

​						amount 大方，新增一行executed，同时改原始行amount-=executed_amount

​	for buyer: change Symbol: add amount

​	for seller: change Account: add balance

