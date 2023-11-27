#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "terminal.h"





EN_terminalError_t getTransactionDate(ST_terminalData_t* termData) {

	if (termData != NULL) {

		printf("PLease enter the transaction date in format (DD/MM/YYYY) : ");
		gets((termData->transactionDate));
		uint8_t length = strlen(termData->transactionDate);
		if ((termData->transactionDate[0] == '\0') || (length != 10) || (termData->transactionDate[2] != '/') 
			|| (termData->transactionDate[5] != '/')) {
			
			return WRONG_DATE;
		}
		else if (!((termData->transactionDate[0] >= '0' && termData->transactionDate[0] <= '3'))) {
			
			return WRONG_DATE;
		}
		else if (!((termData->transactionDate[1] >= '0' && termData->transactionDate[1] <= '9'))) {
			
			return WRONG_DATE;
		}
		else if ((termData->transactionDate[3] != '0') && (termData->transactionDate[3] != '1')) {
		
			return WRONG_DATE;
		}
		else if ((termData->transactionDate[3] == '0') && !(termData->transactionDate[4] >= '1' && termData->transactionDate[4] <= '9')) {
		
			return WRONG_DATE;
		}
		else if ((termData->transactionDate[3] == '1') && !(termData->transactionDate[4] >= '0' && termData->transactionDate[4] <= '2')) {
		
			return WRONG_DATE;
		}
		else if (!((termData->transactionDate[6] == '2') && (termData->transactionDate[7] == '0') && (termData->transactionDate[8] == '2')
				&& (termData->transactionDate[9] == '3'))) {
		
			return WRONG_DATE;
		}
		return TERMINAL_OK;

		
		
	}
	else {
		return INVALID_ADDRESS;
	}
	
	
	

	
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData) {

	if (cardData != NULL && termData != NULL) {



		if (cardData->cardExpirationDate[4] < termData->transactionDate[9]) /* Checks the year */
		{
			return EXPIRED_CARD;
		}
		else if (cardData->cardExpirationDate[4] == termData->transactionDate[9]) {  /* Checks the year if equal */

			if ((cardData->cardExpirationDate[0] == '0') && (termData->transactionDate[3] == '1')) {  /* Checks the Month */
				
				return EXPIRED_CARD;
			}
			else if ((cardData->cardExpirationDate[0] == '0' && (termData->transactionDate[3] == '0'))
				&& (cardData->cardExpirationDate[1] < termData->transactionDate[4])) {
				
				return EXPIRED_CARD;
			}
			else if ((cardData->cardExpirationDate[0] == '1' && (termData->transactionDate[3] == '1'))
				&& (cardData->cardExpirationDate[1] < termData->transactionDate[4])) {
				
				return EXPIRED_CARD;
			}
			else {
				return TERMINAL_OK;
			}
		}
		else {
			return TERMINAL_OK;
		}
			}

	else {
		return INVALID_ADDRESS;
	}

	}


EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData) {

	if (termData != NULL) {

		printf("PLease enter the transaction amount : ");
		scanf("%f", &(termData->transAmount));
		fgetc(stdin);
	
		if (termData->transAmount <= 0.0) {
			
			return INVALID_AMOUNT;
		}
		else {
			
			return TERMINAL_OK;
		}

	}
	else {
		return INVALID_ADDRESS;
	}


}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData) {

	if (termData != NULL) {



		if (termData->transAmount > termData->maxTransAmount ) {
			return EXCEED_MAX_AMOUNT;
		}
		else {
			return TERMINAL_OK;
		}

	}
	else {
		return INVALID_ADDRESS;
	}


}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount) {

	if (termData != NULL) {

		if (maxAmount <= 0.0) {
			return INVALID_MAX_AMOUNT;
		}
		else {
			termData->maxTransAmount = maxAmount;
			return TERMINAL_OK;
		}

	}
	else {
		return INVALID_ADDRESS;
	}


}

int printterminaldetails(ST_terminalData_t* termData) {

	if (termData != NULL) {

		printf("The Date is : %s \n", termData->transactionDate);
		printf("The max Transaction Amount is : %f \n", termData->maxTransAmount);
		printf("The Transaction Amount is : %f \n", termData->transAmount);
		return 0;
	}
	else {
		return 1;
	}
}