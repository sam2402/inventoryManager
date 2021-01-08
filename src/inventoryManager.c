// Samuel Lubrano Lavadera 2020

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// Menu options
#define ENTER_ITEM 1
#define UPDATE_STOCK 2
#define SEARCH 3
#define REPORT 4
#define DELETE 5
#define QUIT 6

// Search options
#define NAME 1
#define BARCODE 2
#define PRICE 3
#define STOCK_LEVEL 4

#define MAX_LINE_LENGTH 40
#define INVENTORY_SIZE 1000

typedef struct item
{
    char *name;
    char *barcode;
    float price;
    int stockLevel;
} item;

// functions from phonebook.c
item *inventory[INVENTORY_SIZE];
void go(void);
void displayMenu(void);
void doOption(int);
int getMenuOption(void);
char *readString(char *);

// helper functions I wrote
void createNewItem(char *, char *, float, int);
void deleteItemWithBarcode(char *);
void getInventoryFromFile(void);
void updateInventoryFromModel(void);
int getIndexOfNextFreeInventoryPtr(void);
int getIndexOfItemWithBarcode(char *);
void printHeader(void);
void printItemWithBarcode(char *);
int isDuplicateBarcode(char *);
void freeInventoryMemory(void);
int inventoryIsFull(void);

//search options
int *searchName(void);
int *searchBarcode(void);
int *searchPrice(void);
int *searchStockLevel(void);

// user options - one of these is called whenever a user enters a number in the options menu
void enterItem(void);
void updateStock(void);
void search(void);
void report(void);
void delete (void);

int main()
{
    go();
    return 0;
}

// asks the user to enter an option to perform some task until they quit the program
void go()
{
    while (true)
    {
        displayMenu();
        int option = getMenuOption();
        if (option == QUIT)
        {
            break;
        }
        else
        {
            doOption(option);
        }
    }
}

void displaySearchMenu()
{
    printf("Would you like to search by:\n");
    printf("1. Name\n");
    printf("2. Barcode\n");
    printf("3. Price\n");
    printf("4. Stock Level\n");
}

void displayMenu()
{
    printf("\nInventory Manager\n");
    printf("%i. Enter a new item\n", ENTER_ITEM);
    printf("%i. Record or update the number of a given item in stock\n", UPDATE_STOCK);
    printf("%i. Search for an item\n", SEARCH);
    printf("%i. Produce a report of all items in stock \n", REPORT);
    printf("%i. Delete an item\n", DELETE);
    printf("%i. Quit\n", QUIT);
}

int getMenuOption()
{
    int option = 0;
    char *input = readString("Enter selection: ");
    sscanf(input, "%i", &option);
    free(input);
    return option;
}

// the caller should free the returned value from this function at an appropriate time
char *readString(char *prompt)
{
    char buffer[MAX_LINE_LENGTH];
    printf("%s", prompt);
    fgets(buffer, MAX_LINE_LENGTH, stdin);
    size_t inputLength = strlen(buffer);
    char *input = calloc(sizeof(char), inputLength);
    strncpy(input, buffer, inputLength - 1); //stores the user's input in the buffer
    input[inputLength] = '\0';
    return input;
}

void doOption(int option)
{
    switch (option)
    {
    case ENTER_ITEM:
        enterItem();
        break;
    case UPDATE_STOCK:
        updateStock();
        break;
    case SEARCH:
        search();
        break;
    case REPORT:
        report();
        break;
    case DELETE:
        delete ();
        break;
    default:
        printf("\nInvalid option entered, try again\n");
    }
}

// fills an array of length len with -1
// the caller should free the returned array at an appropriate time
int *initiliseArray(int len)
{
    int *arr = malloc(len * sizeof(int));
    for (int i = 0; i < len; i++)
    {
        arr[i] = -1;
    }
    return arr;
}

// returns an array of integers that correspond to the indices of inventory where matching items have been found
// returns NULL if no items are found
int *doSearchOption(int option)
{
    int *ret;
    switch (option)
    {
    case NAME:
        ret = searchName();
        break;
    case BARCODE:
        ret = searchBarcode();
        break;
    case PRICE:
        ret = searchPrice();
        break;
    case STOCK_LEVEL:
        ret = searchStockLevel();
        break;
    default:
        ret = NULL;
        printf("\nInvalid option entered\n");
    }
    return ret;
}

// the next four functions are called by doSearchOption() to perform its respective search
// they return an array of integers that correspond to the indices of inventory where matching items have been found
int *searchName()
{
    char *inpName = readString("Enter the name of the item(s) you are searching for: ");
    int *matchingNameIndecies = initiliseArray(INVENTORY_SIZE);
    int nextFreeIndex = 0;
    int index = 0;
    while (inventory[index] != NULL) // while there are still unread items in inventory
    {
        // if the user inputted name and the indexed inventory item name are equal,
        // append the index to matchingNameIndecies
        if (strcmp(inventory[index]->name, inpName) == 0)
        {
            matchingNameIndecies[nextFreeIndex] = index;
            nextFreeIndex++;
        }
        index++;
    }
    free(inpName);
    return matchingNameIndecies;
}

int *searchBarcode()
{
    char *inpBarcode = readString("Enter the barcode of the item(s) you are searching for: ");
    int *matchingBarcodeIndecies = initiliseArray(INVENTORY_SIZE);
    int nextFreeIndex = 0;
    int index = 0;
    while (inventory[index] != NULL)
    {
        if (strcmp(inventory[index]->barcode, inpBarcode) == 0)
        {
            matchingBarcodeIndecies[nextFreeIndex] = index;
            nextFreeIndex++;
        }
        index++;
    }
    free(inpBarcode);
    return matchingBarcodeIndecies;
}

int *searchPrice()
{
    char *inpPrice = readString("Enter the price of the item(s) you are searching for: ");
    int *matchingPriceIndecies = initiliseArray(INVENTORY_SIZE);
    int nextFreeIndex = 0;
    int index = 0;
    while (inventory[index] != NULL)
    {
        if (inventory[index]->price == atof(inpPrice))
        {
            matchingPriceIndecies[nextFreeIndex] = index;
            nextFreeIndex++;
        }
        index++;
    }
    free(inpPrice);
    return matchingPriceIndecies;
}

int *searchStockLevel()
{
    char *inpStockLevel = readString("Enter the stock level of the item(s) you are searching for: ");
    int *matchingStockLevelIndecies = initiliseArray(INVENTORY_SIZE);
    int nextFreeIndex = 0;
    int index = 0;
    while (inventory[index] != NULL)
    {
        if (inventory[index]->stockLevel == atoi(inpStockLevel))
        {
            matchingStockLevelIndecies[nextFreeIndex] = index;
            nextFreeIndex++;
        }
        index++;
    }
    free(inpStockLevel);
    return matchingStockLevelIndecies;
}

// creates a new item from the parameters and stores it in the inventory variable
// the caller should take care to free name and barcode at an appropriate time
void createNewItem(char *name, char *barcode, float price, int stockLevel)
{
    if (inventoryIsFull())
    {
        printf("Error: inventory full - item %s not added\n", barcode);
        return;
    }

    item *newItem = calloc(sizeof(item), 1);

    newItem->name = calloc(sizeof(char), strlen(name) + 1);
    strcpy(newItem->name, name);

    newItem->barcode = calloc(sizeof(char), strlen(barcode) + 1);
    strcpy(newItem->barcode, barcode);

    newItem->price = price;
    newItem->stockLevel = stockLevel;

    int index = getIndexOfNextFreeInventoryPtr();
    inventory[index] = newItem;
}

void deleteItemWithBarcode(char *barcode)
{
    int delIndex = getIndexOfItemWithBarcode(barcode);
    free(inventory[delIndex]->name);
    free(inventory[delIndex]->barcode);
    free(inventory[delIndex]);

    for (int index = delIndex; index < getIndexOfNextFreeInventoryPtr(); index++)
    {
        inventory[index] = inventory[index + 1]; // shifts all elements back one overwriting the value to be deleted
    }
}

int inventoryIsFull()
{
    if (inventory[INVENTORY_SIZE - 1] == NULL)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void printHeader()
{
    printf("\n%-12s %-12s %-12s %-12s\n", "ITEM NAME", "BARCODE", "PRICE", "STOCK LEVEL");
}

// returns the index of the item with the given barcode in inventory
// prerequisite: the inventory global variable must already be filled with data
int getIndexOfItemWithBarcode(char *barcode)
{
    int inventoryIndex = 0;
    while (inventory[inventoryIndex] != NULL)
    {
        if (strcmp(inventory[inventoryIndex]->barcode, barcode) == 0)
        {
            return inventoryIndex;
        }
        inventoryIndex++;
    }
    return -1;
}

// prerequisite: the inventory global variable must already be filled with data
void printItemWithBarcode(char *barcode)
{
    int index = getIndexOfItemWithBarcode(barcode);
    if (index != -1)
    {
        item *item = inventory[index];
        printf("%-12s %-12s £%-12.2f%-12d\n", item->name, item->barcode, item->price, item->stockLevel);
    }
    else
    {
        printf("No item with barcode %s exists\n", barcode);
    }
}

// prerequisite: the inventory global variable must already be filled with data
int isDuplicateBarcode(char *barcode)
{
    if (getIndexOfItemWithBarcode(barcode) == -1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

// returns the index of the next index in inventory that does not have an item pointer in it
int getIndexOfNextFreeInventoryPtr()
{
    int inventoryIndex = 0;
    while (inventory[inventoryIndex] != NULL)
    {
        inventoryIndex += 1;
    }
    return inventoryIndex;
}

// store the data in inventory.csv in the inventory variable as an array of pointers to items
void getInventoryFromFile()
{
    FILE *file = fopen("../datastore/inventory.csv", "r");
    char line[MAX_LINE_LENGTH];
    char *name;
    char *barcode;
    float price;
    int stockLevel;
    while (fgets(line, sizeof(line), file))
    { // while there are still lines left to read in file
        char *token;
        token = strtok(line, ","); // split the line up with "," as the separator
        int feildCount = 0;
        while (token != NULL)
        {
            switch (feildCount)
            {
            case 0:
                name = token;
                break;
            case 1:
                barcode = token;
                break;
            case 2:
                price = atof(token);
                break;
            case 3:
                stockLevel = atoi(token);
                break;
            }
            token = strtok(NULL, ",");
            feildCount++;
        }
        createNewItem(name, barcode, price, stockLevel);
    }

    // set the rest of the elements in inventory to NULL
    int index = getIndexOfNextFreeInventoryPtr();
    while (index < INVENTORY_SIZE)
    {
        inventory[index] = NULL;
        index++;
    }
    fclose(file);
}

// updates the data in the inventory.csv file to match what is stored in the inventory variable
void updateInventoryFromModel()
{
    FILE *file = fopen("../datastore/inventory.csv", "w");
    for (int inventoryIndex = 0; inventoryIndex < getIndexOfNextFreeInventoryPtr(); inventoryIndex++)
    {
        item *item = inventory[inventoryIndex];
        fprintf(file, "%s,%s,%f,%d\n", item->name, item->barcode, item->price, item->stockLevel);
    }
    fclose(file);
}

void freeInventoryMemory()
{
    int inventoryIndex = 0;
    while (inventory[inventoryIndex] != NULL)
    {
        free(inventory[inventoryIndex]->name);
        inventory[inventoryIndex]->name = NULL;
        free(inventory[inventoryIndex]->barcode);
        inventory[inventoryIndex]->barcode = NULL;
        free(inventory[inventoryIndex]);
        inventory[inventoryIndex] = NULL;
        inventoryIndex++;
    }
}

// Each of the remaining functions corresponds to one of the menu options
void enterItem()
{
    getInventoryFromFile();
    if (inventoryIsFull())
    {
        printf("The inventory is full, you can't add any more items.\n");
        freeInventoryMemory();
        return;
    }
    char *inpBarcode = readString("Enter the barcode of the new item: ");
    if (isDuplicateBarcode(inpBarcode))
    {
        printf("A product with that barcode is already in your inventory:\n\n");
        printHeader();
        printItemWithBarcode(inpBarcode);
        freeInventoryMemory();
        return;
    }
    char *inpName = readString("Enter the name of the new item: ");
    char *inpPrice = readString("Enter the price of the new item: ");
    char *inpStockLevel = readString("Enter the stock level of the new item: ");
    createNewItem(inpName, inpBarcode, atof(inpPrice), atoi(inpStockLevel));
    updateInventoryFromModel();
    printf("Item added to your inventory:\n");
    printHeader();
    printItemWithBarcode(inpBarcode);
    free(inpBarcode);
    free(inpName);
    free(inpPrice);
    free(inpStockLevel);
    freeInventoryMemory();
}

void updateStock()
{
    getInventoryFromFile();
    char *inpBarcode = readString("Enter the barcode of the item whose stock level you would like to change: ");
    if (!isDuplicateBarcode(inpBarcode))
    {
        printf("No item with barcode %s is in your inventory.\n", inpBarcode);
        freeInventoryMemory();
        return;
    }
    int itemIndex = getIndexOfItemWithBarcode(inpBarcode);
    char *inpStockLevel = readString("Enter the number of the item in stock: ");
    inventory[itemIndex]->stockLevel = atoi(inpStockLevel); // change the model
    updateInventoryFromModel();  // update this change in the file
    printf("Stock level changed\n");
    printHeader();
    printItemWithBarcode(inpBarcode);
    free(inpBarcode);
    free(inpStockLevel);
    freeInventoryMemory();
}

void search()
{
    getInventoryFromFile();
    displaySearchMenu();
    int inpSearchOption = getMenuOption();

    //an array of itegers that correspond to the indecies of inventory where matching items have been found
    int *matchingIndecies = doSearchOption(inpSearchOption);
    if (matchingIndecies == NULL) // if the user entered a wrong option (not between 1 and 4)
    {
        free(matchingIndecies);
        freeInventoryMemory();
        return;
    }
    if (matchingIndecies[0] == -1) //if no items have been found
    {
        printf("No items found.\n");
    }
    else
    {
        printHeader();
        int index = 0;
        while (matchingIndecies[index] != -1) // while there are still unread items items in matchingIndecies
        {
            // print a table row of a matching item
            int inventoryIndex = matchingIndecies[index];
            printItemWithBarcode(inventory[inventoryIndex]->barcode);
            index++;
        }
    }
    free(matchingIndecies);
    freeInventoryMemory();
}

void report()
{
    getInventoryFromFile();
    int inventoryIndex = 0;
    printHeader();
    while (inventory[inventoryIndex] != NULL)
    {
        printItemWithBarcode(inventory[inventoryIndex]->barcode);
        ++inventoryIndex;
    }
    freeInventoryMemory();
    printf("\n");
}

void delete ()
{
    getInventoryFromFile();
    char *inpBarcode = readString("Enter the barcode of the item you would like remove from the inventory: ");
    if (!isDuplicateBarcode(inpBarcode))
    {
        printf("No item with barcode %s is in the inventory.\n", inpBarcode);
        freeInventoryMemory();
        return;
    }
    printf("'%s", inventory[getIndexOfItemWithBarcode(inpBarcode)]->name);
    deleteItemWithBarcode(inpBarcode);
    updateInventoryFromModel();
    printf("' has been removed from the inventory\n");
    free(inpBarcode);
    freeInventoryMemory();
}