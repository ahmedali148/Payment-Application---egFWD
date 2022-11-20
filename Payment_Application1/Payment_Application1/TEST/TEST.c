#include "../CARD/Card.h"
#include "../TERMINAL/Terminal.h"
#include "../SERVER/Server.h"

void TEST_Start(void)
{
   /*_____________*Card Module*____________*/
	//getCardHolderNameTest();
	//getCardExpiryDateTest();
	//getCardPANTest();

	/*_____________*Terminal Module*________*/
	//getTransactionDateTest();
	isCardExpiredTest();
	//isValidCardPANTest();
	//getTransactionAmountTest();
	setMaxAmountTest();
	isBelowMaxAmountTest();

	/*_____________*Server Module*__________*/
	isValidAccountTest();
	isBlockedAccountTest();
	isAmountAvailableTest();
	saveTransactionTest();
	recieveTransactionDataTest();
}