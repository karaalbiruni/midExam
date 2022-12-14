#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <conio.h>
#include <assert.h>
#include <ctype.h>
#define SIZE 10

void mainMenu();
void cekSystem();
void cekTime();
void addDish();
void removeDish();
void addCustomer();
void searchCustomer();
void viewWarteg();
void order();

struct hashNode{
    char name[255];
    int key;
};
hashNode *table[SIZE];

void init()
{
    for (int i = 0; i < SIZE; i++)
    {
        table[i] = NULL;
    }
}

int hashFunction(char name[])
{
    int sum = 0, hashKey;
    for (int i = 0; i < strlen(name); i++)
    {
        sum = sum + name[i];
    }

    hashKey = sum % SIZE;
    return hashKey;
}

void insertHash(char name[])
{
    int hashKey = hashFunction(name);
    hashNode *temp = (hashNode *)malloc(sizeof(hashNode));
    temp->key = hashKey;
    strcpy(temp->name, name);
    if (table[hashKey] == NULL)
    {
        table[hashKey] = temp;
    }
    else
    {
        int index = hashKey + 1;
        if (index == SIZE)
        {
            index = 0;
        }

        while (index != hashKey)
        {
            if (table[index] == NULL)
            {
                table[index] = temp;
                temp->key = index;
                return;
            }
            else
            {
                index++;
                if (index == SIZE)
                {
                    index = 0;
                }
            }
        }

        printf("Table is full..\n");
    }
}

int searchHash(char name[])
{
    int hashKey = hashFunction(name);
    if (table[hashKey] != NULL && strcmp(table[hashKey]->name, name) == 0)
    {
        return hashKey;
    }
    else
    {
        int index = hashKey + 1;
        if (index == SIZE)
        {
            index = 0;
        }
        while (index != hashKey)
        {
            if (table[index] != NULL && strcmp(table[index]->name, name) == 0)
            {
                return index;
            }
            else{
                index++;
                if (index == SIZE)
                {
                    index = 0;
                }
            }
        }

        return -1;
    }
}

void deleteHash(char name[])
{
    int benersalah = searchHash(name);

    if (benersalah == -1)
    {
        printf("Name is not found..\n");
    }
    else
    {
        free(table[benersalah]);
        table[benersalah] = NULL;
    }
}

void printData()
{
    for (int i = 0; i < SIZE; i++)
    {
        if (table[i] != NULL)
        {
            printf("%d. %s\n", table[i]->key, table[i]->name);
        }
    }
    puts("");
}

struct nodeDish{
    char name[255];
    int price, quantity;
    struct nodeDish *next;
    struct nodeDish *prev;
};
nodeDish *head = NULL;
nodeDish *tail = NULL;

struct nodeDish *createNodeDish(const char name[], int price, int quantity)
{
    nodeDish *temp = (nodeDish *)malloc(sizeof(nodeDish));
    strcpy(temp->name, name);
    temp->price = price;
    temp->quantity = quantity;
    temp->next = NULL;
    return temp;
}

void AddDish(char name[], int price, int quantity)
{
    nodeDish *temp = createNodeDish(name, price, quantity);
    if (head == NULL)
    {
        head = tail = temp;
    }
    else
    {
        head->prev = temp;
        temp->next = head;
        temp->prev = NULL;
        head = temp;
    }
}

void ViewDish(nodeDish *temp)
{
    temp = head;

    int a = 1;

    puts("Bude's Menu");
    puts("============================================================================");
    printf("| %-2s | %-20s | %-20s | %-20s |\n", "No.", "Name", "Quantity", "Price");
    puts("============================================================================");
    while(temp)
    {
        printf("| %-2d  | %-20s | %-20d | %-20d |\n", a++, temp->name, temp->quantity, temp->price);
        temp = temp->next;
    }
    puts("============================================================================");
}

nodeDish *searchDish(char name[])
{
    if (head == NULL)
    {
        return NULL;
    }
    if (strcmp(head->name, name) == 0)
    {
        return head;
    }
    else if (strcmp(tail->name, name) == 0)
    {
        return tail;
    }
    nodeDish *ptr = head;
    while (ptr != NULL && strcmp(ptr->name, name) != 0)
    {
        ptr = ptr->next;
    }
    return ptr;
}

void popHead()
{
    if (head == tail)
    {
        nodeDish *temp = head;
        free(temp);
        head = tail = NULL;
        return;
    }
    nodeDish *temp = head;
    head = head->next;
    free(temp);
    return;
}

void popTail()
{
    if (head == tail)
    {
        nodeDish *temp = tail;
        free(temp);
        head = tail = NULL;
        return;
    }
    nodeDish *prev = NULL;
    nodeDish *temp = head;
    while (temp->next != NULL)
    {
        prev = temp;
        temp = temp->next;
    }
    tail = prev;
    prev->next = NULL;
    free(temp);
    return;
}

int RemoveDish(char name[])
{
    nodeDish *temp = searchDish(name);
    if (temp->name == NULL)
    {
        return -1;
    }

    if (strcmp(temp->name, name) == 0)
    {
        popHead();
        printf("The dish has been removed!\n");
        printf("Press enter to continue... ");
        getchar();
        mainMenu();
        return 1;
    }
    else if (strcmp(tail->name, name) == 0)
    {
        popTail();
        printf("The dish has been removed!\n");
        printf("Press enter to continue... ");
        getchar();
        mainMenu();
        return 1;
    }

    temp = head;;
    nodeDish *prev = NULL;
    while (temp->next != NULL && strcmp(temp->name, name) != 0)
    {
        prev = temp;
        temp = temp->next;
    }
    prev->next = temp->next;
    free(temp);
}

int main()
{
    init();
    mainMenu();

    return 0;
}

void mainMenu()
{
    system("cls");
    int choice;
    cekSystem();
    cekTime();
    puts("1. Add Dish");
    puts("2. Remove Dish");
    puts("3. Add Customer");
    puts("4. Search Customer");
    puts("5. View Warteg");
    puts("6. Order");
    puts("7. Payment");
    puts("8. Exit Warteg");
    do
    {
        printf(">> ");
        scanf("%d", &choice);
        getchar();
    } 
    while (choice < 1 || choice > 8);
    
    if (choice == 1)
    {
        addDish();
    }
    else if (choice == 2)
    {
        removeDish();
    }
    else if (choice == 3)
    {
        addCustomer();
    }
    else if (choice == 4)
    {
        searchCustomer();
    }
    else if (choice == 5)
    {
        viewWarteg();
    }
    else if (choice == 6)
    {
        order(); // blm masuk ke hash table
    }
    else if (choice == 7)
    {

    }
    else if (choice == 8)
    {
        system("cls");
        FILE* fpread = fopen("keluar.txt", "r");
        char ch;

        do{
            ch = fgetc(fpread);
            printf("%c", ch);
        } while (ch != EOF);
        exit(0);
    }
}

void order()
{
    system("cls");
    char name[255];
    int jumlah;

    printf("Insert the customer's name: ");
    scanf("%[^\n]", name); getchar();

    int res = searchHash(name);

    if (res == -1)
    {
        system("cls");
        puts("Customer's not present");
        printf("Press enter to continue... "); getchar();
        mainMenu();
    }
    else 
    {
        if (head == NULL)
        {
            puts("No dish was found!");
            printf("Press enter to continue... "); getchar();
            mainMenu();
        }
        else
        {
            puts("");
            ViewDish(head);

            printf("Insert the amount of dish: ");
            scanf("%d", &jumlah); getchar();

            char dishName[jumlah + 2][255];
            int quantity[jumlah + 2];

            for (int i = 0; i < jumlah; i++)
            {
                printf("[%d]Insert the dish's name and quantity: ", i + 1);
                scanf("%[^\n] x %d", dishName[i], quantity[i]);
            }
        }
    }
}

void viewWarteg()
{
    system("cls");
    puts("Customer's List");
    printData();
    printf("Press enter to continue... ");
    getchar();
    mainMenu();
}

void searchCustomer()
{
    system("cls");
    char customerName[255];
    int cek;

    do
    {
        cek = 0;
        printf("Insert the customer's name to be searched: ");
        scanf("%[^\n]", customerName); getchar();

        for (int i = 0; i < strlen(customerName); i++)
        {
            if (isspace(customerName[i]) != 0)
            {
                cek++;
            }
        }
    } while (cek > 0);

    int res = searchHash(customerName);

    if (res != -1)
    {
        printf("%s is present\n", customerName);
    }
    else if (res == -1)
    {
        printf("%s is not present\n", customerName);
    }

    printf("Press enter to continue... ");
    getchar();
    mainMenu();
    
}

void addCustomer()
{
    system("cls");
    char customerName[255];
    int cek;

    do
    {
        cek = 0;
        printf("Insert the customer's name [Without space]: ");
        scanf("%[^\n]", customerName); getchar();

        for (int i = 0; i < strlen(customerName); i++)
        {
            if (isspace(customerName[i]) != 0)
            {
                cek++;
            }
        }
    } while (cek > 0);

    insertHash(customerName);

    puts("Customer has been added!");
    printf("Press enter to continue... ");
    getchar();
    mainMenu();
    
}

void removeDish()
{
    system("cls");

    if (head == NULL)
    {
        puts("No dish was found!");
        printf("Press enter to continue... "); getchar();
        mainMenu();
    }
    else
    {
        char name[255];
        ViewDish(head);

        printf("Insert dish's name to be deleted: ");
        scanf("%[^\n]", name); getchar();

        RemoveDish(name);

        printf("Dish not found!\n");
        printf("Press enter to continue... ");
        getchar();
        mainMenu();
    }
}

void addDish()
{
    system("cls");
    char name[255];
    int price, quantity;
    int count;
    
    do
    {
        printf("Insert the name of the dish [Lowercase letters]: ");
        scanf("%[^\n]", name); getchar();

        count = 0;

        for (int i = 0; i < strlen(name); i++)
        {
            if (name[i] >= 'A' && name[i] <= 'Z')
            {
                count++;
            } 
        }
    } 
    while (count > 0);

    do
    {
        printf("Insert the price of the dish [1000..50000]: ");
        scanf("%d", &price); getchar();
    } while (price < 1000 || price > 50000);

    do
    {
        printf("Insert the quantity of the dish [1..999]: ");
        scanf("%d", &quantity); getchar();
    } while (quantity < 1 || quantity > 999);

    AddDish(name, price, quantity);

    printf("Press enter to continue... "); getchar();
    mainMenu();
}

void cekTime()
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    printf("%s", asctime(tm));
}

void cekSystem()
{
    #ifdef _WIN32
    puts("System: Windows OS");
    #elif __APPLE__
    puts("System: Mac OS");
    #elif __linux__
    puts("System: Linux OS");
    #elif TARGET_OS_EMBEDDED
    puts("System: iOS embedded OS");
    #elif TARGET_IPHONE_SIMULATOR
    puts("System: iOS simulator OS");
    #elif TARGET_OS_IPHONE
    puts("System: iPhone OS");
    #elif TARGET_OS_MAC
    puts("System: MAC OS");
    #elif __ANDROID__
    puts("System: android OS");
    #elif __unix__
    puts("System: unix OS");
    #elif _POSIX_VERSION
    puts("System: POSIX based OS");
    #elif __sun
    puts("System: Solaris OS");
    #elif __hpux
    puts("System: HP UX OS");
    #elif BSD
    puts("System: Solaris OS");
    #elif __DragonFly__
    puts("System: DragonFly BSD OS");
    #elif __FreeBSD__
    puts("System: FreeBSD OS");
    #elif __OpenBSD__
    puts("System: Open BSD OS");
    #endif
}