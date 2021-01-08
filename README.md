# Inventory Manager

##### How to run Inventory Manager:
- Fork this repository
- run the commands below:
`cd src`
`gcc inventoryManager.c`
`./a.out`
- I have included a file called inventory.csv in the datastore directory that contains sample data.

##### How to use Inventory Manager:
- Follow the instructions on screen.
- Enter numbers that correspond to tasks.
- Enter 6 to end the program when prompted.

##### What the application allows the user to do:
- Enter an item (name, barcode, price, stock)
- Record the number in stock
- Update the number in stock
- Search for an item by name, barcode, price or stock level
- Produce a report of all items in stock
- Delete an item

##### Notes on the design:
- The data is stored in a csv file called inventory.csv.
- I have taken design ideas from the MVC design paradigm.
    - A global variable called inventory is filled with items from the inventory.csv file
    - The user's task is performed (this may include updating the file to keep it in sync with the inventory global variable)
    - The memory holding inventory is freed
    - Effectively, the csv file is the model, the inventory global variable is a controller and the view is functions called by doOption. (Although this is very loose and the doOption functions also act as controllers)
- The barcode is used as each item's unique identifier.

##### Features to look out for:
- There is validation throughout the program. For example:
    - If the user enters something which doesn't correspond to a valid option, an error message is displayed 
    - The user shouldn't be able to enter a new item if the inventory is full
    - The user can't reference the barcode of an item that doesn't exist
- In the search option, if multiple items match the name, price or stock level, then all are returned. ("jam" occurs three times in the sample data file).
    

