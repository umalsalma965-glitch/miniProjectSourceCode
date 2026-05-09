#include <stdio.h>
#include <stdlib.h>

struct account
{
    int accNo;
    char name[30];
    float balance;
};

void addAccount();
void displayAccounts();
void deposit();
void withdraw();
void deleteAccount();

int main()
{
    int choice;

    while (1)
    {
        printf("\n----- BANK MANAGEMENT SYSTEM -----\n");
        printf("1. Add Account\n");
        printf("2. Display All Accounts\n");
        printf("3. Deposit Money\n");
        printf("4. Withdraw Money\n");
        printf("5. Delete Account\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addAccount();
            break;

        case 2:
            displayAccounts();
            break;

        case 3:
            deposit();
            break;

        case 4:
            withdraw();
            break;

        case 5:
            deleteAccount();
            break;

        case 6:
            printf("Program Ended\n");
            exit(0);

        default:
            printf("Invalid Choice\n");
        }
    }

    return 0;
}

void addAccount()
{
    FILE *fp;
    struct account a;

    fp = fopen("bank.dat", "ab");

    printf("Enter Account Number: ");
    scanf("%d", &a.accNo);

    printf("Enter Name: ");
    scanf("%s", a.name);

    printf("Enter Balance: ");
    scanf("%f", &a.balance);

    fwrite(&a, sizeof(a), 1, fp);
    fclose(fp);

    printf("Account Added Successfully\n");
}

void displayAccounts()
{
    FILE *fp;
    struct account a;

    fp = fopen("bank.dat", "rb");

    if (fp == NULL)
    {
        printf("No records found\n");
        return;
    }

    printf("\nAccNo\tName\tBalance\n");

    while (fread(&a, sizeof(a), 1, fp))
    {
        printf("%d\t%s\t%.2f\n", a.accNo, a.name, a.balance);
    }

    fclose(fp);
}

void deposit()
{
    FILE *fp;
    struct account a;
    int acc;
    float amount;

    fp = fopen("bank.dat", "rb+");

    printf("Enter Account Number: ");
    scanf("%d", &acc);

    while (fread(&a, sizeof(a), 1, fp))
    {
        if (a.accNo == acc)
        {
            printf("Enter Amount to Deposit: ");
            scanf("%f", &amount);

            a.balance += amount;

            fseek(fp, -sizeof(a), SEEK_CUR);
            fwrite(&a, sizeof(a), 1, fp);

            printf("Deposit Successful\n");
            fclose(fp);
            return;
        }
    }

    printf("Account Not Found\n");
    fclose(fp);
}

void withdraw()
{
    FILE *fp;
    struct account a;
    int acc;
    float amount;

    fp = fopen("bank.dat", "rb+");

    printf("Enter Account Number: ");
    scanf("%d", &acc);

    while (fread(&a, sizeof(a), 1, fp))
    {
        if (a.accNo == acc)
        {
            printf("Enter Amount to Withdraw: ");
            scanf("%f", &amount);

            if (amount > a.balance)
            {
                printf("Insufficient Balance\n");
            }
            else
            {
                a.balance -= amount;

                fseek(fp, -sizeof(a), SEEK_CUR);
                fwrite(&a, sizeof(a), 1, fp);

                printf("Withdrawal Successful\n");
            }

            fclose(fp);
            return;
        }
    }

    printf("Account Not Found\n");
    fclose(fp);
}

void deleteAccount()
{
    FILE *fp, *temp;
    struct account a;
    int acc;
    int found = 0;

    fp = fopen("bank.dat", "rb");
    temp = fopen("temp.dat", "wb");

    printf("Enter Account Number to Delete: ");
    scanf("%d", &acc);

    while (fread(&a, sizeof(a), 1, fp))
    {
        if (a.accNo != acc)
        {
            fwrite(&a, sizeof(a), 1, temp);
        }
        else
        {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("bank.dat");
    rename("temp.dat", "bank.dat");

    if (found)
        printf("Account Deleted Successfully\n");
    else
        printf("Account Not Found\n");
}