#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../CARD/Card.h"
#include "../TERMINAL/Terminal.h"
#include "Server.h"

/*transaction data object*/
ST_transaction_t transDataObj;

/*account data base object*/
ST_accountsDB_t accountRefrence;

/*Data base array of ST_accountsDB_t*/
static ST_accountsDB_t AccountsDataBase[255] =
{
	{2000, RUNNING, "8989374615436851\n"},
	{10000, BLOCKED, "5807007076043875\n"},
	{12000, RUNNING, "4417123456789113\n"},
	{3700, BLOCKED, "2614978996959267\n"},
	{4500, RUNNING, "2479189249803973\n"},
	{3000, RUNNING, "5412751234123456\n"},
	{7800, RUNNING, "4000233158759123\n"}
};

/*Data base array of  ST_transaction_t*/
static ST_transaction_t transactionDataBase[255] ={ 0 };

/*Global Counter*/
static uint16_t Global_Counter = 0;

static uint32_t sequenceNumber;

/*Global array to print Enum states as string*/
const char* getEnumState[] = { "APPROVED", "DECLINED_INSUFFECIENT_FUND", "DECLINED_STOLEN_CARD", "FRAUD_CARD", "INTERNAL_SERVER_ERROR" };

/*____________________isValidAccount_____________________*/
/*This function will take card dataand validate if the account related to this card exists or not.
 *It checks if the PAN exists or not in the server's database (searches for the card PAN in the DB).
 *If the PAN doesn't exist will return ACCOUNT_NOT_FOUND and the account reference will be NULL, else will return SERVER_OK and return a reference to this account in the DB.
*/
EN_serverError_t isValidAccount(ST_cardData_t* cardData)
{
	/*This function will take card data and validate these data.*/
	EN_serverError_t Local_return = SERVER_OK;
	uint16_t Local_Counter = 0;
	for (Local_Counter = 0; Local_Counter < 255; Local_Counter++)
	{

		if (!(strcmp((cardData->primaryAccountNumber), (AccountsDataBase[Local_Counter].primaryAccountNumber))))
		{
			Local_return = SERVER_OK;
			Global_Counter = Local_Counter;
			break;
		}
		else
		{
			Local_return = ACCOUNT_NOT_FOUND;
		}
	}
	return Local_return;
}

void isValidAccountTest(void)
{
	printf("Tester Name: Ahmed Ali \nFunction Name: isValidAccount\n");
	EN_serverError_t ValidAccountTester = isValidAccount(&transDataObj.cardHolderData);
	if (ValidAccountTester == SERVER_OK)
	{
		printf("Test Valid Account: Valid \n");
		puts("_____________________________");
	}
	else
	{
		printf("Test Valid Account: Invalid \n");
		puts("_____________________________");
	}
}

/*__________________________isBlockedAccount_________________*/
/*This function takes a reference to the account into the database and verifies if it is blocked or not.
 *If the account is running it will return SERVER_OK, else if the account is blocked it will return BLOCKED_ACCOUNT.
*/
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	if ((AccountsDataBase[Global_Counter].AccountState) == BLOCKED)
	{
		return BLOCKED_ACCOUNT;
	}
	else
	{
		return SERVER_OK;
	}
}

void isBlockedAccountTest(void)
{
	printf("Tester Name: Ahmed Ali \nFunction Name: isBlockedAccount\n");
	EN_serverError_t BlockedAccountTester = isBlockedAccount(&accountRefrence);
	if (BlockedAccountTester == SERVER_OK)
	{
		printf("Test Blocked Account: Valid \n");
		puts("_____________________________");
	}
	else
	{
		printf("Test Blocked Account: Invalid \n");
		puts("_____________________________");
	}
}

/*_____________________isAmountAvailable___________________*/
/*This function will take terminal data and a reference to the account in the database and check if the account has a sufficient amount to withdraw or not.
 *It checks if the transaction's amount is available or not.
 *If the transaction amount is greater than the balance in the database will return LOW_BALANCE, else will return SERVER_OK.
*/
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData)
{
	if (((uint16_t)termData->transAmount) > (AccountsDataBase[Global_Counter].balance))
	{
		return LOW_BALANCE;
	}
	else
	{
		return SERVER_OK;
	}
}

void isAmountAvailableTest(void)
{
	printf("Tester Name: Ahmed Ali \nFunction Name: isAmountAvailable\n");
	EN_serverError_t AmountAvailableTester = isAmountAvailable(&transDataObj.terminalData);
	if (AmountAvailableTester == SERVER_OK)
	{
		printf("Test Amount Available: Valid \n");
		puts("_____________________________");
	}
	else
	{
		printf("Test Amount Available: Invalid \n");
		puts("_____________________________");
	}
}

/*_______________________saveTransaction___________________*/
/*This function will store all transaction data in the transactions database.
* It gives a sequence number to a transaction, this number is incremented once a transaction is processed into the server, you must check the last sequence number in the server to give the new transaction a new sequence number.
* It saves any type of transaction, APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, FRAUD_CARD, INTERNAL_SERVER_ERROR.
* It will list all saved transactions using the listSavedTransactions function.
* Assuming that the connection between the terminal and server is always connected, then it will return SERVER_OK
*/
EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
		if (sequenceNumber < 255)
		{
			strcpy_s(transactionDataBase[sequenceNumber].cardHolderData.cardHolderName, 25,transData->cardHolderData.cardHolderName);
			strcpy_s(transactionDataBase[sequenceNumber].cardHolderData.cardExpirationDate, 7, transData->cardHolderData.cardExpirationDate);
			strcpy_s(transactionDataBase[sequenceNumber].cardHolderData.primaryAccountNumber, 26,transData->cardHolderData.primaryAccountNumber);
			strcpy_s(transactionDataBase[sequenceNumber].terminalData.transactionDate, 11, transData->terminalData.transactionDate);
			transactionDataBase[sequenceNumber].terminalData.maxTransAmount = transData->terminalData.maxTransAmount;
			transactionDataBase[sequenceNumber].terminalData.transAmount = transData->terminalData.transAmount;
			transactionDataBase[sequenceNumber].transactionSequenceNumber = sequenceNumber;
			transactionDataBase[sequenceNumber].transState = APPROVED;
			sequenceNumber++;

			return SERVER_OK;
		}
		else
		{
			return SAVING_FAILED;
		}
}

void saveTransactionTest(void)
{
	printf("Tester Name: Ahmed Ali \nFunction Name: saveTransaction\n");
	EN_serverError_t saveTransactionTester = saveTransaction(&transDataObj);
	if (saveTransaction == SERVER_OK)
	{
		printf("Test save Transaction: Valid \n");
		puts("_____________________________");
	}
	else
	{
		printf("Test save Transaction: Invalid \n");
		puts("_____________________________");
	}
}

/*__________________________recieveTransactionData___________________*/
/*This function will take all transaction data and validate its data, it contains all server logic.
 *It checks the account details and amount availability.
 *If the account does not exist return FRAUD_CARD, if the amount is not available will return DECLINED_INSUFFECIENT_FUND, if the account is blocked will return DECLINED_STOLEN_CARD, if a transaction can't be saved will return INTERNAL_SERVER_ERROR , else returns APPROVED.
 *It will update the database with the new balance.
*/
EN_transState_t recieveTransactionData(ST_transaction_t* transData, ST_accountsDB_t* accountRefrence)
{
	EN_transState_t Local_return = APPROVED;

	if ((isValidAccount(&transData->cardHolderData)) == ACCOUNT_NOT_FOUND)
	{
		Local_return = FRAUD_CARD;
	}
	else if ((isAmountAvailable(&transData->terminalData)) == LOW_BALANCE)
	{
		Local_return = DECLINED_INSUFFECIENT_FUND;
	}
	else if ((isBlockedAccount(&accountRefrence[Global_Counter]))== BLOCKED_ACCOUNT)
	{
		Local_return = DECLINED_STOLEN_CARD;
	}
	else
	{
		Local_return = APPROVED;
		AccountsDataBase[Global_Counter].balance -= transData->terminalData.transAmount;
	}
	transDataObj.transState = Local_return;
	return Local_return;
}

void recieveTransactionDataTest(void)
{
	printf("Tester Name: Ahmed Ali \nFunction Name: recieveTransactionData\n");
	EN_transState_t TransactionDataTester = recieveTransactionData(&transDataObj, &accountRefrence);
	if (TransactionDataTester == APPROVED)
	{
		printf("Test Below Max Amount: APPROVED \n");
		puts("_____________________________");
	}
	else if(TransactionDataTester == FRAUD_CARD)
	{
		printf("Test Below Max Amount: Account not found \n");
		puts("_____________________________");
	}
	else if (TransactionDataTester == DECLINED_INSUFFECIENT_FUND)
	{
		printf("Test Below Max Amount: Low Balance \n");
		puts("_____________________________");
	}
	else if (TransactionDataTester == DECLINED_STOLEN_CARD)
	{
		printf("Test Below Max Amount: Blocked Account \n");
		puts("_____________________________");
	}
}

/*_________________________listSavedTransactions____________________*/
/*This function prints all transactions found in the transactions DB*/
void listSavedTransactions(void)
{
	puts("####################################################");
	printf("Transation Sequence Number: %d\n", transDataObj.transactionSequenceNumber);
	printf("Transation Date: %s\n", transDataObj.terminalData.transactionDate);
	printf("Transation Amount: %d\n", transDataObj.terminalData.transAmount);
	printf("Transation State: %s\n", getEnumState[transDataObj.transState]);
	printf("Terminal Max Amount: %d\n", transDataObj.terminalData.maxTransAmount);
	printf("Card Holder Name: %s\n", transDataObj.cardHolderData.cardHolderName);
	printf("PAN: %s\n", transDataObj.cardHolderData.primaryAccountNumber);
	printf("Card Expiration Date: %s\n", transDataObj.cardHolderData.cardExpirationDate);
	puts("________________________________________________");
	printf("- - - Your Account Balance: %d - - -\n", AccountsDataBase[Global_Counter].balance);
	puts("________________________________________________");
	puts("#####################################################");
}