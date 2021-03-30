# ECE568 HW3 Report



## TODO: Simply explain parser



## Database:

We designed three tables in our database.

They are as follows:

ACCOUNT(<u>ACCOUNT_ID</u>, BALANCE)

SYMBOL(<u>SYM</u>, <u>OWNER</u>,AMOUNT)

ORDER(<u>ID</u>, SYM, AMOUNT, PRICE, TYPE, STATUS, ACCOUNT_ID, TRANS_ID, TIME)

The owner in symbol has a foreign key relationship to account_id in account.

The account_id in order also has a foreign key relationship to account_id in account.

### Create

##### `createAccount(int id, double balance)`

We would check whether this id already exists in account table and also check whether balance is negative.

##### `createSymbol(string name, int owner, int amount)`

We would check whether this owner exists in account and check if amount is negative.

If the primary key (name+owner) already exist, we would add the amount into this record.

### Transcation

#### Order

##### `createOrder(string name, double amount, double price, int account_id)`

If the amount is negative, we set its type to sell.

If the amount is positive, we set its type to buy.

For sell order, we would check whether this user has enough symbols to sell. If enough, we would minus the amount from this symbol record.

For buy order, we would check whether this user has enough balance to buy. If enough, we would minus balance from this user's balance.

And we would do match order every time a new order is opened.

##### `matchSellOrder(string name, double amount, double price, int account_id,int trans_id)`

To match a sell order, we would search proper buy orders (price>=sell_price) and order them by price DESC and transaction id ASC. 

##### `matchBuyOrder(string name, double amount, double price, int account_id,int trans_id)`

To match a buy order, we would search proper sell orders (price<=buy_price) and order them by price ASC and transaction id ASC. 

##### Match process

When two orders can be matched. If they has equal amount, we just change the status to executed and set time of these two orders.

If one side has amount left, we would set add new line for this order to be executed, and minus amount on the origin order.

#### Query

##### `queryOrder(int trans_id, int account_id)`

We would first check if this trans_id exists and belongs to this account_id.

If not, we would send an error message.

Then we would select from orders, and transfer the result into required format.

#### Cancel

##### `cancelOrder(int trans_id, int account_id)`

We would first check if this trans_id belongs to this account_id.

If not, we would send an error message.

And then we would change open order status to canceled and set time.

Then we would call queryOrder to show the result.



## Test

#### Correctness : TODO!!!

**TODO!!!**



#### Scalability

##### Method

We write a test.sh to have multiple client run at the same time and each client would send 1000 requests.

We got the time for executing all the request by query the first orde and last order and see their time, so as to calculate the running time.

1. ##### Order 1000 times per client for same amount

We first let per client to send 1000 order request with the same account. And we got this request.

(We run each mount of request of each kind of core for 5 times and calculate average)

| request\core | 1 core | 2 cores | 4 cores |
| ------------ | ------ | ------- | ------- |
| 4000         | 16s    | 14.6s   | 14.4s   |
| 8000         | 19.3s  | 18.75s  | 55s     |
| 12000        | 28s    | 28s     | 28.75s  |
| 16000        | 38s    | 37.3s   | 36.25s  |
| 20000        | 47.3s  | 46s     | 47s     |

Then we found that there is little difference among one core, two cores and four cores.

We analyzed the reason and guess it is because that we are executing orders and operating on the same account at multiple threads, which means that the database will lock our threads. In this way, we are just running like on one core.

2. ##### Query 1000 time per client

Then we changed our test strategy to make multiple query requests.

However, we still got pretty same performance for one core, two cores and four cores.

| request\core | 1 core | 2 cores | 4 cores |
| ------------ | ------ | ------- | ------- |
| 4000         | 2.6s   | 2.6s    | 2.6s    |
| 8000         | 5.6s   | 5s      | 7s      |
| 16000        | 10.6s  | 10.6s   | 12.3s   |
| 32000        | 21.6s  | 22.3s   | 21s     |

We read some materials and thought maybe we need set the transaction_level of our database to serialized. So we set transaction_level to serialized for every connection and got the following results.

3. ##### Query 1000 time per client : serialized transaction level

| request\core | 1 core | 2 cores | 4 cores |
| ------------ | ------ | ------- | ------- |
| 4000         | 2.6s   | 3.6s    | 3.3s    |
| 8000         | 5.6s   | 5.3s    | 7s      |
| 16000        | 11.3s  | 16.5s   | 12.6s   |
| 32000        | 22.3s  | 23s     | 21s     |
| 48000        | 43s    | 38.6s   | 36.6s   |
| 64000        | 49.3s  | 49s     | 49s     |

However, it still showed no difference for differen cores. We thought it may because our request amount is not big enough, so we let each client send 10,000 requests and have 64 clients connected to server. But the result is also quite similar among one core, two cores and four cores. The running time is around 500 seconds.

