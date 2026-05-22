#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define NAME_LENGTH 50
#define PHONE_LENGTH 20
#define ADDRESS_LENGTH 100
#define TYPE_LENGTH 10

// Account structure definition
typedef struct
{
    int accountNumber;
    char customerName[NAME_LENGTH];
    int age;
    char phoneNumber[PHONE_LENGTH];
    char address[ADDRESS_LENGTH];
    char accountType[TYPE_LENGTH];
    double initialDeposit;
    double currentBalance;
    double gstPercentage;
    double gstAmount;
} Account;

Account accounts[MAX_ACCOUNTS];
int accountCount = 0;

// Function prototypes
void clearInputBuffer(void);
void loadFromFile(void);
void saveToFile(void);
int findAccountIndex(int accountNumber);
void createAccount(void);
void displayAccounts(void);
void searchAccount(void);
void depositMoney(void);
void withdrawMoney(void);
void checkBalance(void);
void updateAccount(void);
void deleteAccount(void);
void calculateGST(void);
void printAccount(const Account *acct);
void readLine(char *buffer, int size);

int main(void)
{
    int choice;

    loadFromFile();

    do
    {
        printf("\n===== BANK MANAGEMENT SYSTEM =====\n");
        printf("1. Create New Account\n");
        printf("2. Display All Accounts\n");
        printf("3. Search Account\n");
        printf("4. Deposit Money\n");
        printf("5. Withdraw Money\n");
        printf("6. Check Balance\n");
        printf("7. Update Account Details\n");
        printf("8. Delete Account\n");
        printf("9. GST Calculation\n");
        printf("10. Save Data\n");
        printf("11. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice)
        {
            case 1:
                createAccount();
                break;
            case 2:
                displayAccounts();
                break;
            case 3:
                searchAccount();
                break;
            case 4:
                depositMoney();
                break;
            case 5:
                withdrawMoney();
                break;
            case 6:
                checkBalance();
                break;
            case 7:
                updateAccount();
                break;
            case 8:
                deleteAccount();
                break;
            case 9:
                calculateGST();
                break;
            case 10:
                saveToFile();
                break;
            case 11:
                saveToFile();
                printf("\nExiting program. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please enter a number between 1 and 11.\n");
                break;
        }
    } while (choice != 11);

    return 0;
}

// Clear extra input characters from stdin
void clearInputBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        ;
    }
}

// Read a line of text safely and remove newline
void readLine(char *buffer, int size)
{
    if (fgets(buffer, size, stdin) != NULL)
    {
        size_t length = strlen(buffer);
        if (length > 0 && buffer[length - 1] == '\n')
        {
            buffer[length - 1] = '\0';
        }
    }
}

// Load account data from binary file
void loadFromFile(void)
{
    FILE *file = fopen("accounts.dat", "rb");
    if (file == NULL)
    {
        accountCount = 0;
        return;
    }
    fread(&accountCount, sizeof(int), 1, file);
    fread(accounts, sizeof(Account), MAX_ACCOUNTS, file);
    fclose(file);
}

// Save account data to binary file
void saveToFile(void)
{
    FILE *file = fopen("accounts.dat", "wb");
    if (file == NULL)
    {
        printf("\nError: Could not save data to file.\n");
        return;
    }
    fwrite(&accountCount, sizeof(int), 1, file);
    fwrite(accounts, sizeof(Account), MAX_ACCOUNTS, file);
    fclose(file);
    printf("\nData saved successfully.\n");
}

// Find index of account in the array by account number
int findAccountIndex(int accountNumber)
{
    for (int i = 0; i < accountCount; i++)
    {
        if (accounts[i].accountNumber == accountNumber)
        {
            return i;
        }
    }
    return -1;
}

// Display a single account
void printAccount(const Account *acct)
{
    printf("\nAccount Number : %d\n", acct->accountNumber);
    printf("Customer Name  : %s\n", acct->customerName);
    printf("Age            : %d\n", acct->age);
    printf("Phone Number   : %s\n", acct->phoneNumber);
    printf("Address        : %s\n", acct->address);
    printf("Account Type   : %s\n", acct->accountType);
    printf("Initial Deposit: %.2f\n", acct->initialDeposit);
    printf("Balance        : %.2f\n", acct->currentBalance);
    printf("GST Percentage : %.2f%%\n", acct->gstPercentage);
    printf("GST Amount     : %.2f\n", acct->gstAmount);
}

// Create a new bank account
void createAccount(void)
{
    if (accountCount >= MAX_ACCOUNTS)
    {
        printf("\nCannot create more accounts. The bank is full.\n");
        return;
    }

    Account newAccount = {0};
    int accountNumber;

    printf("\nEnter new account number: ");
    scanf("%d", &accountNumber);
    clearInputBuffer();

    if (accountNumber <= 0)
    {
        printf("Invalid account number. It must be positive.\n");
        return;
    }

    if (findAccountIndex(accountNumber) != -1)
    {
        printf("Account number %d already exists.\n", accountNumber);
        return;
    }

    newAccount.accountNumber = accountNumber;
    printf("Enter customer name: ");
    readLine(newAccount.customerName, NAME_LENGTH);

    printf("Enter age: ");
    scanf("%d", &newAccount.age);
    clearInputBuffer();
    if (newAccount.age <= 0)
    {
        printf("Age must be greater than 0.\n");
        return;
    }

    printf("Enter phone number: ");
    readLine(newAccount.phoneNumber, PHONE_LENGTH);

    printf("Enter address: ");
    readLine(newAccount.address, ADDRESS_LENGTH);

    printf("Enter account type (Savings or Current): ");
    readLine(newAccount.accountType, TYPE_LENGTH);
    if (strlen(newAccount.accountType) == 0)
    {
        strcpy(newAccount.accountType, "Savings");
    }

    printf("Enter initial deposit amount: ");
    scanf("%lf", &newAccount.initialDeposit);
    clearInputBuffer();
    if (newAccount.initialDeposit < 0)
    {
        printf("Deposit amount cannot be negative.\n");
        return;
    }

    newAccount.currentBalance = newAccount.initialDeposit;

    printf("Select GST percentage (0, 5, 12, 18): ");
    scanf("%lf", &newAccount.gstPercentage);
    clearInputBuffer();
    if (newAccount.gstPercentage != 0 && newAccount.gstPercentage != 5 && newAccount.gstPercentage != 12 && newAccount.gstPercentage != 18)
    {
        printf("Invalid GST percentage. Using 0%%.\n");
        newAccount.gstPercentage = 0;
    }

    newAccount.gstAmount = (newAccount.initialDeposit * newAccount.gstPercentage) / 100.0;

    accounts[accountCount++] = newAccount;
    saveToFile();
    printf("\nNew account created successfully.\n");
}

// Display all stored accounts
void displayAccounts(void)
{
    if (accountCount == 0)
    {
        printf("\nNo accounts found.\n");
        return;
    }

    printf("\n===== All Bank Accounts =====\n");
    for (int i = 0; i < accountCount; i++)
    {
        printAccount(&accounts[i]);
    }
}

// Search an account by account number
void searchAccount(void)
{
    int accountNumber;
    printf("\nEnter account number to search: ");
    scanf("%d", &accountNumber);
    clearInputBuffer();

    int index = findAccountIndex(accountNumber);
    if (index == -1)
    {
        printf("Account number %d was not found.\n", accountNumber);
    }
    else
    {
        printf("\nAccount found:\n");
        printAccount(&accounts[index]);
    }
}

// Deposit money into an account
void depositMoney(void)
{
    int accountNumber;
    double amount;
    printf("\nEnter account number for deposit: ");
    scanf("%d", &accountNumber);
    clearInputBuffer();

    int index = findAccountIndex(accountNumber);
    if (index == -1)
    {
        printf("Account number %d not found.\n", accountNumber);
        return;
    }

    printf("Enter amount to deposit: ");
    scanf("%lf", &amount);
    clearInputBuffer();
    if (amount <= 0)
    {
        printf("Deposit amount must be greater than zero.\n");
        return;
    }

    accounts[index].currentBalance += amount;
    saveToFile();
    printf("\nDeposit successful. New balance: %.2f\n", accounts[index].currentBalance);
}

// Withdraw money from an account
void withdrawMoney(void)
{
    int accountNumber;
    double amount;
    printf("\nEnter account number for withdrawal: ");
    scanf("%d", &accountNumber);
    clearInputBuffer();

    int index = findAccountIndex(accountNumber);
    if (index == -1)
    {
        printf("Account number %d not found.\n", accountNumber);
        return;
    }

    printf("Enter amount to withdraw: ");
    scanf("%lf", &amount);
    clearInputBuffer();
    if (amount <= 0)
    {
        printf("Withdrawal amount must be greater than zero.\n");
        return;
    }

    if (amount > accounts[index].currentBalance)
    {
        printf("Cannot withdraw %.2f. Available balance is %.2f.\n", amount, accounts[index].currentBalance);
        return;
    }

    accounts[index].currentBalance -= amount;
    saveToFile();
    printf("\nWithdrawal successful. Remaining balance: %.2f\n", accounts[index].currentBalance);
}

// Check account balance only
void checkBalance(void)
{
    int accountNumber;
    printf("\nEnter account number to check balance: ");
    scanf("%d", &accountNumber);
    clearInputBuffer();

    int index = findAccountIndex(accountNumber);
    if (index == -1)
    {
        printf("Account number %d not found.\n", accountNumber);
    }
    else
    {
        printf("\nAccount Number: %d\n", accounts[index].accountNumber);
        printf("Current Balance: %.2f\n", accounts[index].currentBalance);
    }
}

// Update customer details in an account
void updateAccount(void)
{
    int accountNumber;
    printf("\nEnter account number to update: ");
    scanf("%d", &accountNumber);
    clearInputBuffer();

    int index = findAccountIndex(accountNumber);
    if (index == -1)
    {
        printf("Account number %d not found.\n", accountNumber);
        return;
    }

    printf("\nUpdating account %d. Leave blank to keep current value.\n", accountNumber);
    printf("Current name: %s\n", accounts[index].customerName);
    printf("Enter new customer name: ");
    char input[ADDRESS_LENGTH];
    readLine(input, NAME_LENGTH);
    if (strlen(input) > 0)
    {
        strcpy(accounts[index].customerName, input);
    }

    printf("Current age: %d\n", accounts[index].age);
    printf("Enter new age (0 to keep current): ");
    int newAge;
    scanf("%d", &newAge);
    clearInputBuffer();
    if (newAge > 0)
    {
        accounts[index].age = newAge;
    }

    printf("Current phone: %s\n", accounts[index].phoneNumber);
    printf("Enter new phone number: ");
    readLine(input, PHONE_LENGTH);
    if (strlen(input) > 0)
    {
        strcpy(accounts[index].phoneNumber, input);
    }

    printf("Current address: %s\n", accounts[index].address);
    printf("Enter new address: ");
    readLine(input, ADDRESS_LENGTH);
    if (strlen(input) > 0)
    {
        strcpy(accounts[index].address, input);
    }

    printf("Current account type: %s\n", accounts[index].accountType);
    printf("Enter new account type: ");
    readLine(input, TYPE_LENGTH);
    if (strlen(input) > 0)
    {
        strcpy(accounts[index].accountType, input);
    }

    printf("Current GST percentage: %.2f%%\n", accounts[index].gstPercentage);
    printf("Enter new GST percentage (0, 5, 12, 18): ");
    double newGST;
    scanf("%lf", &newGST);
    clearInputBuffer();
    if (newGST == 5 || newGST == 12 || newGST == 18)
    {
        accounts[index].gstPercentage = newGST;
        accounts[index].gstAmount = (accounts[index].currentBalance * newGST) / 100.0;
    }

    saveToFile();
    printf("\nAccount details updated successfully.\n");
}

// Delete an account from storage
void deleteAccount(void)
{
    int accountNumber;
    printf("\nEnter account number to delete: ");
    scanf("%d", &accountNumber);
    clearInputBuffer();

    int index = findAccountIndex(accountNumber);
    if (index == -1)
    {
        printf("Account number %d not found.\n", accountNumber);
        return;
    }

    for (int i = index; i < accountCount - 1; i++)
    {
        accounts[i] = accounts[i + 1];
    }

    accountCount--;
    saveToFile();
    printf("\nAccount number %d deleted successfully.\n", accountNumber);
}

// Calculate GST for a service or transaction amount
void calculateGST(void)
{
    double amount;
    double gstPercent;

    printf("\nEnter service or transaction amount: ");
    scanf("%lf", &amount);
    clearInputBuffer();
    if (amount < 0)
    {
        printf("Amount cannot be negative.\n");
        return;
    }

    printf("Enter GST percentage (5, 12, 18): ");
    scanf("%lf", &gstPercent);
    clearInputBuffer();
    if (gstPercent != 5 && gstPercent != 12 && gstPercent != 18)
    {
        printf("Invalid GST percentage. Please use 5, 12, or 18.\n");
        return;
    }

    double gstAmount = (amount * gstPercent) / 100.0;
    double totalAmount = amount + gstAmount;

    printf("\nGST Calculation Result:\n");
    printf("------------------------\n");
    printf("Service Amount : %.2f\n", amount);
    printf("GST Percentage : %.2f%%\n", gstPercent);
    printf("GST Amount     : %.2f\n", gstAmount);
    printf("Total Amount   : %.2f\n", totalAmount);
}
