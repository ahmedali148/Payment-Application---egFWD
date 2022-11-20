#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../CARD/Card.h"
#include "../TERMINAL/Terminal.h"
#include "../SERVER/Server.h"
#include "app.h"

/*transaction data object*/
ST_transaction_t transDataObj;

/*account data base object*/
ST_accountsDB_t accountRefrence;


void appStart(void)
{
	/*___________**__________* Card module *_____________**___________*/

	/*check -Card Holder Name-, if return WRONG_NAME print wrong Name and try again
	 *if return CARD_OK complete the application.
	 */
	if ((getCardHolderName(&transDataObj.cardHolderData)) == WRONG_NAME)
	{
		puts("Wrong Name, Try again ");
		getCardHolderName(&transDataObj.cardHolderData);
	}

	/*check -Card Expiration Date-, if return WRONG_EXP_DATE print wrong format and try again
     * if return CARD_OK complete the application.
     */

	if ((getCardExpiryDate(&transDataObj.cardHolderData)) == WRONG_EXP_DATE)
	{
		puts("Wrong Format, Try again  with this format MM/YY ");
		getCardExpiryDate(&transDataObj.cardHolderData);
	}

	/*check -Card PAN-, if return WRONG_PAN print wrong and try again
     * if return CARD_OK complete to check if number is luhn number or not
     * if number not luhn number print Invalid  luhn PAN and ask user to try again.
     */

	if ((getCardPAN(&transDataObj.cardHolderData)) == WRONG_PAN)
	{
		puts("Wrong PAN, Try again ");
		getCardPAN(&transDataObj.cardHolderData);
	}
	else 
	{
		//EN_terminalError_t isValidPANChecker = isValidCardPAN(&transDataObj.cardHolderData);
		if (isValidCardPAN(&transDataObj.cardHolderData) == INVALID_CARD)
		{
			puts("Invalid Luhn PAN, Try again");
			getCardPAN(&transDataObj.cardHolderData );
		}
		else 
		{
			puts("Valid Luhn PAN");
			puts("______________________________________________________");
		}
	}

	/*______________**______________* Terminal module *______________**___________*/

	getTransactionDate(&transDataObj.terminalData);

	/*check is Card Expaired*/
	if ((isCardExpired(&transDataObj.cardHolderData, &transDataObj.terminalData)) == EXPIRED_CARD)
	{
		puts("________Declined Expired Card________ ");
		exit(0);
	}

	/*check Max Amount if it Invalid ask user to set it again*/
	//EN_terminalError_t maxAmountChecker = setMaxAmount(&transDataObj.terminalData);
	if ((setMaxAmount(&transDataObj.terminalData)) == INVALID_MAX_AMOUNT)
	{
		puts("_______Invalid Max Amount, Try again________ ");
		setMaxAmount(&transDataObj.terminalData);
	}
	else
	{
		/*check transaction Amount if it Invalid print this*/
		//EN_terminalError_t getTransAmountChecker = getTransactionAmount(&transDataObj.terminalData);
		if ((getTransactionAmount(&transDataObj.terminalData)) == INVALID_AMOUNT)
		{
			puts("___________Invalid Amount___________");
			exit(0);
		}
		else
		{
			/*check if transaction amount <= max amount*/
			//EN_terminalError_t belowMaxAmountChecker = isBelowMaxAmount(&transDataObj.terminalData);
			if (isBelowMaxAmount(&transDataObj.terminalData) == EXCEED_MAX_AMOUNT) {
				puts("________Declined Amount Exceeding Limit___________");
				exit(0);
			}
			else
			{
				puts("_____________________________________________________");
			}
		}
	}

	/*____________**___________Server module___________**__________*/

	
	/*Save the Transaction Data*/
	saveTransaction(&transDataObj);

	/*check server returns and return the acount state*/
	EN_transState_t recieveTransDataChecker = recieveTransactionData(&transDataObj, &accountRefrence);
	if (recieveTransDataChecker == FRAUD_CARD)
	{
		puts("_____________DECLIINED FRAUD_CARD_____________");
	}
	else if (recieveTransDataChecker == DECLINED_INSUFFECIENT_FUND)
	{
		puts("__________DECLINED_INSUFFECIENT_FUND__________");
	}
	else if (recieveTransDataChecker == DECLINED_STOLEN_CARD)
	{
		puts("______________DECLINED_STOLEN_CARD____________");
	}
	else if (recieveTransDataChecker == INTERNAL_SERVER_ERROR) {
		puts("______________INTERNAL_SERVER_ERROR___________");
	}
	else if (recieveTransDataChecker == APPROVED)
	{
		puts("_______________Transaction Completed____________");
		puts("________________ Updating Account_______________");
		puts("_______________Transaction is Saved ____________");
		listSavedTransactions();
		exit(0);
	}
}
