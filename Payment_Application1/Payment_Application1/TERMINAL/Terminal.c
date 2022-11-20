#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../CARD/Card.h"
#include "../TERMINAL/Terminal.h"

uint8_t test_mode1 = 0;

/*terminal data object*/
ST_terminalData_t terminalDataObj;

/*Card data object*/
ST_cardData_t cardDataObj;

/*___________________getTransactionDate_________________*/
/*This function will ask for the transaction date and store it in terminal data.
 *Transaction date is 10 characters string in the format DD/MM/YYYY, e.g 25/06/2022.
 *If the transaction date is NULL or is less than 10 characters or wrong format will return the WRONG_DATE error, else return TERMINAL_OK.
*/
EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	printf("Please Enter the transaction date in form DD/MM/YYYY:\n");
	fgets( termData->transactionDate, 11, stdin);
	if ((termData->transactionDate == NULL) || (strlen(termData->transactionDate) < 10) || (strlen(termData->transactionDate) > 10))
		return WRONG_DATE;

	else
		return Terminal_OK;
}

void getTransactionDateTest(void)
{
	printf("Tester Name: Ahmed Ali \nFunction Name: getTransactionDate\n");
	EN_terminalError_t TransactionDateTester = getTransactionDate(&terminalDataObj);
	if (TransactionDateTester == Terminal_OK)
	{
		printf("Test Transaction Date: Valid \n");
		puts("_____________________________");
	}
	else
	{
		printf("Test Transaction Date: Invalid \n");
		puts("_____________________________");
	}
}

/*______________________isCardExpired______________________*/
/*This function compares the card expiry date with the transaction date.
 *If the card expiration date is before the transaction date will return EXPIRED_CARD, else return TERMINAL_OK.
*/
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	/*cardData->cardExpirationDate "MM/YY"
	* termData->transactionDate    "DD/MM/YYYY"
	*/
	uint8_t Local_return;
	if ((cardData->cardExpirationDate[3]) == (termData->transactionDate[8]))
	{
		if ((cardData->cardExpirationDate[4]) == (termData->transactionDate[9]))
		{
			if ((cardData->cardExpirationDate[0]) == (termData->transactionDate[3]))
			{
				if ((cardData->cardExpirationDate[1]) >= (termData->transactionDate[4]))
				{
					Local_return = Terminal_OK;
				}
				else
					Local_return = EXPIRED_CARD;
			}
			else if ((cardData->cardExpirationDate[0]) > (termData->transactionDate[3]))
			{
				Local_return = Terminal_OK;
			}
			else
				Local_return = EXPIRED_CARD;
		}
		else if ((cardData->cardExpirationDate[4]) > (termData->transactionDate[9]))
		{
			Local_return = Terminal_OK;
		}
		else
			Local_return = EXPIRED_CARD;
	}
	else if ((cardData->cardExpirationDate[3]) > (termData->transactionDate[8]))
	{
		Local_return = Terminal_OK;
	}
	else
	{
		Local_return = EXPIRED_CARD;
	}
	return Local_return;
}
void isCardExpriedTest(void) {
	printf("Tester Name: Ahmed Ali \nFunction Name: isCardExpired\n");
	ST_terminalData_t terminal_data;
	ST_cardData_t card_data;
	EN_terminalError_t error;
	const char* errors[] = { "TERMINAL_OK", "WRONG_DATE", "EXPIRED_CARD",
			"INVALID_CARD", "INVALID_AMOUNT", "EXCEED_MAX_AMOUNT",
			"INVALID_MAX_AMOUNT" };
	test_mode = 1;

	printf("Test case 1\n");
	strcpy(card_data.cardExpirationDate, "05/25");
	strcpy(terminal_data.transactionDate, "26/06/2026");
	error = isCardExpired(&card_data, &terminal_data);
	printf("Input text : Card Expiry Date %s Transaction Date %s\n",
		card_data.cardExpirationDate, terminal_data.transactionDate);
	printf("Expected result : %s\n", errors[2]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 2\n");
	strcpy(card_data.cardExpirationDate, "05/25");
	strcpy(terminal_data.transactionDate, "16/10/2024");
	error = isCardExpired(&card_data, &terminal_data);
	printf("Input text : Card Expiry Date %s Transaction Date %s\n",
		card_data.cardExpirationDate, terminal_data.transactionDate);
	printf("Expected result : %s\n", errors[0]);
	printf("Actual result : %s\n", errors[error]);

	printf("Test case 3\n");
	strcpy(card_data.cardExpirationDate, "05/25");
	strcpy(terminal_data.transactionDate, "26/05/2025");
	error = isCardExpired(&card_data, &terminal_data);
	printf("Input text : Card Expiry Date %s Transaction Date %s\n",
		card_data.cardExpirationDate, terminal_data.transactionDate);
	printf("Expected result : %s\n", errors[0]);
	printf("Actual result : %s\n", errors[error]);

//void isCardExpiredTest(void)
//{
//	printf("Tester Name: Ahmed Ali \nFunction Name: isCardExpired\n");
//	EN_terminalError_t CardExpiredTester = isCardExpired(&cardDataObj, &terminalDataObj);
//	if (CardExpiredTester == Terminal_OK)
//	{
//		printf("Test Card Expiration: Valid \n");
//		puts("_____________________________");
//	}
//	else
//	{
//		printf("Test Card Expiration: Invalid \n");
//		puts("_____________________________");
//	}
//}

/*_______________________isValidCardPAN_____________________*/
/*This function will check if the PAN is a Luhn number or not.
 * If the number is not Luhn number, will return INVALID_CARD, else will return TERMINAL_OK
 *Implement to luhn  algorithm check
 * double even Numbers and check if result >= 10, subtract 9 from result 
 * if sum of the total Number % 10 == 0, PAN is Valid
 */
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
	uint8_t Local_Counter, Local_return = Terminal_OK;
	uint16_t Local_OddSum = 0, Local_EvenSum = 0;

	for (Local_Counter = 0; Local_Counter < (strlen(cardData->primaryAccountNumber)); Local_Counter++)
	{
		if ((Local_Counter % 2) == 0)
		{
			if ((((cardData->primaryAccountNumber[Local_Counter]) - ZERO_ASCI) * 2) >= 10)
			{
				Local_EvenSum += ((((cardData->primaryAccountNumber[Local_Counter]) - ZERO_ASCI) * 2) - 9);
			}
			else
			{
				Local_EvenSum += (((cardData->primaryAccountNumber[Local_Counter]) - ZERO_ASCI) * 2);
			}
		}
		else
		{
			Local_OddSum += ((cardData->primaryAccountNumber[Local_Counter]) -ZERO_ASCI);
		}
		if (((Local_EvenSum + Local_OddSum) % 10) != 0)
		{
			Local_return = INVALID_CARD;
		}
		else
		{
			Local_return = Terminal_OK;
		}
	}
	return Local_return;
}

void isValidCardPANTest(void)
{
	printf("Tester Name: Ahmed Ali \nFunction Name: isValidCardPAN\n");
	EN_terminalError_t ValidCardPANTester = isValidCardPAN(&cardDataObj);
	if (ValidCardPANTester == Terminal_OK)
	{
		printf("Test Valid Card PAN: Valid \n");
		puts("_____________________________");
	}
	else
	{
		printf("Test Valid Card PAN: Invalid \n");
		puts("_____________________________");
	}
}

/*_________________________getTransactionAmount_____________________*/
/*This function asks for the transaction amount and saves it into terminal data.
 *If the transaction amount is less than or equal to 0 will return INVALID_AMOUNT, else return TERMINAL_OK.
 */
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	printf("Please Enter The transaction amount:\n");
	scanf_s("%d", &(termData->transAmount));

	if ((termData->transAmount == 0) || (termData->transAmount < 0))
		return INVALID_AMOUNT;

	else
		return Terminal_OK;
}

void getTransactionAmountTest(void)
{
	printf("Tester Name: Ahmed Ali \nFunction Name: getTransactionAmount\n");
	EN_terminalError_t TransactionAmountTester = getTransactionAmount(&terminalDataObj);
	if (TransactionAmountTester == Terminal_OK)
	{
		printf("Test Card Expiration: Valid \n");
		puts("_____________________________");
	}
	else
	{
		printf("Test Card Expiration: Invalid \n");
		puts("_____________________________");
	}
}

/*_____________________setMaxAmount___________________*/
/*This function takes the maximum allowed amount and stores it into terminal data.
 *Transaction max amount is a float number.
 *If transaction max amount less than or equal to 0 will return the INVALID_MAX_AMOUNT error, else return TERMINAL_OK.
*/
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
	printf("Please Enter  the maximum allowed amount:\n");
	scanf_s("%d", &termData->maxTransAmount);

	if ((termData->maxTransAmount == 0) || (termData->maxTransAmount < 0))
		return INVALID_MAX_AMOUNT;

	else
		return Terminal_OK;
}

void setMaxAmountTest(void)
{
	printf("Tester Name: Ahmed Ali \nFunction Name: setMaxAmount\n");
	EN_terminalError_t MaxAmountTester = setMaxAmount(&terminalDataObj);
	if (MaxAmountTester == Terminal_OK)
	{
		printf("Test Max Amount: Valid \n");
		puts("_____________________________");
	}
	else
	{
		printf("Test Max Amount: Invalid \n");
		puts("_____________________________");
	}
}

/*____________________isBelowMaxAmount________________*/
/*This function compares the transaction amount with the terminal max allowed amount.
 *If the transaction amount is larger than the terminal max allowed amount will return EXCEED_MAX_AMOUNT, else return TERMINAL_OK.
*/
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	
	if ((termData->transAmount) > (termData->maxTransAmount))
		return EXCEED_MAX_AMOUNT;
	else
		return Terminal_OK;
}

void isBelowMaxAmountTest(void)
{
	printf("Tester Name: Ahmed Ali \nFunction Name: isBelowMaxAmount\n");
	EN_terminalError_t BelowMaxAmountTester = isBelowMaxAmount(&terminalDataObj);
	if (BelowMaxAmountTester == Terminal_OK)
	{
		printf("Test Below Max Amount: Valid \n");
		puts("_____________________________");
	}
	else
	{
		printf("Test Below Max Amount: EXCEED_MAX_AMOUNT \n");
		puts("_____________________________");
	}
}