# hw3

### Database

* Symbol: 

  ​	stock or commodity

* Position: 

  ​	symbol+amount, amount could be fractional

  ​	positive: long

  ​	negative(not allowed)

* Account: identified globally unique account number (primary key)
  * current balance (us dollars). ----- never become negative 
  * set of positions (possible empty)

  A transation which would cause a negative account balance cannot be processed

* order
  * buy or sell
  * symbol + amount (positive to purchase, negative to sell) + limit price
  * price : maximum price to buy or minimum price to sell
    * when placed
      * Buy : shares * prices decuted from buyer's account
      * Sell: shares are duducted from seller's account
    * when executed
      * Buy: buyer's account credited the shares of the symbol purchased
      * Sell: the seller's account is credited for the sale price
* open order
  
  * when first placed, it is open
* match order
  * both order are open
  * they are for same symbol
  * their limit prices are compatible : the sell order’s limit price is lower than the buy order’s limit price.

* executing order
  * Seller: adding money to account
  * Buyer: create a new position in buyers account (if already have one, just add amount)
  * Order status change from open to execute
  * they must be done atomatically:
    * changing the seller’s account balance, changing buyer’s number of shares, and altering order status)
  * determine price:  define the execution price to be the price of the order that was open first
* cancle an order:
  *  Once an order is canceled, it MUST NOT match with any other orders and MUST NOT execute.
    * Buy: refound money to account
    * Sell: return shares to account



### server requirment

* first create: no symbol, no account, no order
* Port 12345
* when connection : one line indicate length
  * create
  * transactions

#### create (page 2 end)

* create account  
* create symbol	: may put symbol into many accounts

 children of a create MUST be processed in the order they appear in the input.

* reply
  * Root: result
    * created
    * error
  * created and error MUST have the account id (for account creation) or sym and account id
  * the error tag should have a textual body describing what was wrong – e.g

#### transactions

* Root: account id
* children
  * order
  * cancle
  * query
* process children nodes in any order
* order
  * attributes: sym, Amount, limit
  * Reply:
    * opened
    * Error
    * Both of these will contain the same sym, amount, and limit as the requested transaction, and appear in the same order as the transactions in the input.
    * and the opened will contain an id attribute, indicates the order
    * Error: 
      * have a textual description of the problem.
      * if the account ID is invalid, then an <error> will be reported for each transaction. 
* query
  * Attribute: id of the transaction
  * Reply:
    * Status: attritube id
    * children
      * open shares=
      * canceld shares=, time =
      * Executed shares=, price=, time=
      * Note: open and cancel are exclusive
* Cancel:
  * Attribute: id of the transaction
  * cancel the open order, any executed part is not affected
  * reply
    * Canceled, children
      * canceld shares=, time =
      * Executed shares=, price=, time=
    *  Canceling a buy order refunds the buyer’s account for the cost of the canceled purchase price.
    * **what about sell? add back shares?**

#### match orders

* give the best price match
* break ties by executing order arrive earlier



### submission

* Docker compose
* A writeup, including graphs in which you analyze the scalability of your server.
* Your testing infrastructure, which you used to test your server