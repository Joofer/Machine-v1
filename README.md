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
RuntimeListener* runtime = new RuntimeListener(); // Create runtime class to use some high-level functions, such as Buy function

connection->ConnectToDatabase("tcp://127.0.1.1:3306", "user", "password", "database"); // Connecting to database
runtime->Create(connection); // Creating new machine using previously established connection
runtime->Pick(connection, id); // Picking which machine our Machine, ConnectionHandler and RuntimeListener classes should handle using its database id
wallet->SetMoney(100); // Setting up wallet used for purchases
UpdateData(id, connection, wallet, coffee_machine); // Loading up machine with data got from database, data is stored in Machine (Coffee Machine) class

string item = "Cappuccino";
int error; // Variable for storing errors
error = runtime->Buy(id, wallet, coffee_machine, item, connection); // Buy an item
```
# List of functions
Machine() - Create a new machine
CoffeeMachine(), BeverageMachine(), WendingMachine() - Create a new specific machine
| Set | Get |
|----------------|---------|
| SetName(string) - Set name for machine. Arguments: name | GetName() - Get name of machine. Return: string |
| Add(string, int) - Add a new item to machine. Arguments: item, value (count) | Find(string) - Get item's value (count). Arguments: name. Return: int |
| Take(string, int) - Take some items. Arguments: item, value (count). Return: true/false (bool) | Check() - Check ingredients to be in an enough amount. Return: string (which of the ingredients is missing, key of items map) |
| Delete(string) - Delete an item. Arguments: name. Return true/false (bool) |  |
| Change(string, int) - Change item count. Arguments: item, value (count). Return: true/false (bool) |  |
______
