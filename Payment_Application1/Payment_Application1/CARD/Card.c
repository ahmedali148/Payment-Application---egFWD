#include <stdio.h>
#include <string.h>

#include "Card.h"


/*Card data object*/
ST_cardData_t cardDataObj;

/*__________________getCardHolderName______________*/
/*This function will ask for the cardholder's name and store it into card data.
* Cardholder name is 24 alphabetic characters string max and 20 min.
* If the cardholder name is NULL, less than 20 characters or more than 24 will return a WRONG_NAME error, else return CARD_OK.
*/
EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	printf("Please Enter Card Holder Name:\n");
	fgets(cardData->cardHolderName, 24, stdin);

	if ((cardData->cardHolderName == NULL) || (strlen(cardData->cardHolderName) < 20) || (strlen(cardData->cardHolderName) > 24))
		return WRONG_NAME;

	else
		return CARD_OK;
}

void getCardHolderNameTest(void)
{
	printf("Tester Name: Ahmed Ali \nFunction Name: getCardHolderName\n");
	EN_cardError_t CardHolderNameTester = getCardHolderName(&cardDataObj);
	if (CardHolderNameTester == CARD_OK)
	{
		printf("Test Card Holder Name: Valid \n");
		puts("_____________________________");
	}
	else
	{
		printf("Test Card Holder Name: Invalid \n");
		puts("_____________________________");
	}
}

/*___________________getCardExpiryDate_____________*/
/*This function will ask for the card expiry date and store it in card data.
 *Card expiry date is 5 characters string in the format "MM/YY", e.g "05/25".
 *If the card expiry date is NULL, less or more than 5 characters, or has the wrong format will return the WRONG_EXP_DATE error, else return CARD_OK.
 */
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	puts("Please Enter the card expiry date in form MM/YY: ");
	gets(cardData->cardExpirationDate);

	if ((cardData->cardExpirationDate == NULL) || (strlen(cardData->cardExpirationDate) != 5))
		return WRONG_EXP_DATE;

	else
		return CARD_OK;
}

void getCardExpiryDateTest(void)
{
	printf("Tester Name: Ahmed Ali \nFunction Name: getCardExpiryDate\n");
	EN_cardError_t CardExpiryDateTester = getCardExpiryDate(&cardDataObj);
	if (CardExpiryDateTester == CARD_OK)
	{
		printf("Test Card Expiry Date: Valid \n");
		puts("_____________________________");
	}
	else
	{
		printf("Test Card Expiry Date: invalid \n");
		puts("_____________________________");
	}
}

/*____________________getCardPAN____________________*/
/*This function will ask for the card's Primary Account Number and store it in card data.
 *PAN is 20 numeric characters string, 19 character max, and 16 character min.
 *If the PAN is NULL, less than 16 or more than 19 characters, will return the WRONG_PAN error, else return CARD_OK.
*/
EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	puts("Please Enter the card's Primary Account Number: ");
	fgets(cardData->primaryAccountNumber, 26, stdin);

	if ((cardData->primaryAccountNumber == NULL) || (strlen(cardData->primaryAccountNumber) > 19) || (strlen(cardData->primaryAccountNumber) < 16))
		return WRONG_PAN;

	else
		return CARD_OK;
}

void getCardPANTest(void)
{
	printf("Tester Name: Ahmed Ali \nFunction Name: getCardPAN\n");
	EN_cardError_t CardPANTester = getCardPAN(&cardDataObj);
	if (CardPANTester == CARD_OK)
	{
		printf("Test Card PAN: Valid\n");
		puts("_____________________________");
	}
	else
	{
		printf("Test Card PAN: Invalid \n");
		puts("_____________________________");
	}
}