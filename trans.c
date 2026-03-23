#include <stdio.h>
#include <stdlib.h>

// structure
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// function prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

//  new features
void displayAll(FILE *fPtr);
void searchAccount(FILE *fPtr);
void totalBalance(FILE *fPtr);

int main(int argc, char *argv[])
{
    FILE *cfPtr;
    unsigned int choice;

    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("File could not be opened.\n");
        exit(1);
    }

    while ((choice = enterChoice()) != 5)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;
        case 2:
            updateRecord(cfPtr);
            break;
        case 3:
            newRecord(cfPtr);
            break;
        case 4:
            deleteRecord(cfPtr);
            break;
        case 6:
            displayAll(cfPtr);
            break;
        case 7:
            searchAccount(cfPtr);
            break;
        case 8:
            totalBalance(cfPtr);
            break;
        default:
            printf("Invalid choice\n");
        }
    }

    fclose(cfPtr);
    return 0;
}

// menu
unsigned int enterChoice(void)
{
    unsigned int choice;

    printf("\nEnter your choice\n"
           "1 - store accounts to text file\n"
           "2 - update an account\n"
           "3 - add a new account\n"
           "4 - delete an account\n"
           "5 - exit\n"
           "6 - display all accounts\n"
           "7 - search account\n"
           "8 - total bank balance\n? ");

    scanf("%u", &choice);
    return choice;
}

// create text file
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    struct clientData client;

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        printf("File could not be opened\n");
        return;
    }

    rewind(readPtr);

    fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
            "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr))
    {
        if (client.acctNum != 0)
        {
            fprintf(writePtr, "%-6d%-16s%-11s%10.2f\n",
                    client.acctNum, client.lastName,
                    client.firstName, client.balance);
        }
    }

    fclose(writePtr);
}

// update account
void updateRecord(FILE *fPtr)
{
    unsigned int acc;
    double trans;
    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found\n");
    }
    else
    {
        printf("Current Balance: %.2f\n", client.balance);
        printf("Enter amount (+ deposit / - withdraw): ");
        scanf("%lf", &trans);

        client.balance += trans;

        fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);

        printf("Updated Balance: %.2f\n", client.balance);
    }
}

// add new account
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};
    unsigned int acc;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0)
    {
        printf("Account already exists\n");
    }
    else
    {
        printf("Enter lastname firstname balance: ");
        scanf("%14s%9s%lf", client.lastName, client.firstName, &client.balance);

        client.acctNum = acc;

        fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&client, sizeof(struct clientData), 1, fPtr);
    }
}

// delete account
void deleteRecord(FILE *fPtr)
{
    struct clientData client;
    struct clientData blank = {0, "", "", 0.0};
    unsigned int acc;

    printf("Enter account number to delete: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found\n");
    }
    else
    {
        fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
        fwrite(&blank, sizeof(struct clientData), 1, fPtr);

        printf("Account deleted\n");
    }
}

// display all accounts
void displayAll(FILE *fPtr)
{
    struct clientData client;

    rewind(fPtr);

    printf("\n%-6s%-16s%-11s%10s\n",
           "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-6d%-16s%-11s%10.2f\n",
                   client.acctNum, client.lastName,
                   client.firstName, client.balance);
        }
    }
}

//  search account
void searchAccount(FILE *fPtr)
{
    unsigned int acc;
    struct clientData client;

    printf("Enter account number: ");
    scanf("%u", &acc);

    fseek(fPtr, (acc - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found\n");
    }
    else
    {
        printf("\nAccount Details:\n");
        printf("%-6d%-16s%-11s%10.2f\n",
               client.acctNum, client.lastName,
               client.firstName, client.balance);
    }
}

// total balance
void totalBalance(FILE *fPtr)
{
    struct clientData client;
    double total = 0;

    rewind(fPtr);

    while (fread(&client, sizeof(struct clientData), 1, fPtr))
    {
        if (client.acctNum != 0)
        {
            total += client.balance;
        }
    }

    printf("Total Bank Balance: %.2f\n", total);
}
