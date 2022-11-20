#ifndef SERVER_H_
#define SERVER_H_

typedef enum EN_transState_t
{
	APPROVED, DECLINED_INSUFFECIENT_FUND, DECLINED_STOLEN_CARD, FRAUD_CARD, INTERNAL_SERVER_ERROR
}EN_transState_t;

typedef struct ST_transaction_t
{
	ST_cardData_t cardHolderData;
	ST_terminalData_t terminalData;
	EN_transState_t transState;
	uint32_t transactionSequenceNumber;
}ST_transaction_t;

typedef enum EN_serverError_t
{
	SERVER_OK, SAVING_FAILED, TRANSACTION_NOT_FOUND, ACCOUNT_NOT_FOUND, LOW_BALANCE, BLOCKED_ACCOUNT
}EN_serverError_t;

typedef enum EN_AccountState_t
{
	RUNNING, BLOCKED
}EN_AccountState_t;

typedef struct ST_accountsDB_t
{
	uint32_t balance;
	EN_AccountState_t AccountState;
	uint8_t primaryAccountNumber[26];
}ST_accountsDB_t;

EN_transState_t recieveTransactionData(ST_transaction_t* transData, ST_accountsDB_t* accountRefrence);
EN_serverError_t isValidAccount(ST_cardData_t* cardData);
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence);
EN_serverError_t isAmountAvailable(ST_terminalData_t* termData);
EN_serverError_t saveTransaction(ST_transaction_t* transData);
void listSavedTransactions(void);


void isValidAccountTest(void);
void isBlockedAccountTest(void);
void isAmountAvailableTest(void);
void saveTransactionTest(void);
void recieveTransactionDataTest(void);


#endif