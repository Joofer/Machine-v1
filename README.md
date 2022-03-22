# Depencies
The following libraries have to be installed and included to Visual Studio before running the project
+ [MySQL Connector C++ 1.1.13](https://dev.mysql.com/downloads/connector/cpp/1.1.html)
+ [boost_1_78_0](https://www.boost.org/users/history/version_1_78_0.html)  

Project was built using C++20.
# Wending Machine Libraries
This is a pack of libraries made to create and test wending machine.
It contains of multiple wending machine types:
+ Coffee Machine
+ Beverage Machine
+ Wending Machine
______
# Basic usage
The following code represents how to create ```CoffeeMachine``` and load it up with some cool (hot actually) stuff:
``` C++
Machine* coffee_machine = new CoffeeMachine(); // Creating a new machine of type Coffee Machine
Wallet* wallet = new Wallet(); // Create a wallet for purchases
ConnectionHandler* connection = new ConnectionHandler(); // Create MySQL connection class
int id; // Machine's id

connection->ConnectToDatabase("tcp://127.0.1.1:3306", "user", "password", "database"); // Connecting to database
Runtime::Create(connection); // Creating new machine using previously established connection
Runtime::Pick(connection, id); // Picking which machine our Machine, ConnectionHandler and Runtime classes should handle using its database id
wallet->SetMoney(100); // Setting up the wallet used for purchases
UpdateData(id, connection, wallet, coffee_machine); // Loading up machine with data got from database, data is stored in Machine (Coffee Machine) class

string item = "Cappuccino";
int error; // Variable for storing errors
Runtime::Buy(id, wallet, coffee_machine, item, connection, error); // Buy an item, sets error code to the error variable, -1 if succeed
```
