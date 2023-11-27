#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "card.h"

EN_cardError_t getCardHolderName(ST_cardData_t* cardData) {

	if (cardData != NULL) {
	
		printf("Please enter your name : ");
		gets((cardData->cardHolderName));
		uint8_t length = strlen(cardData->cardHolderName);

		if ( (cardData->cardHolderName[0] == '\0') || ((length < 20 || length > 24)) ) {
			
			return WRONG_NAME;
		}
		else {

			for (int index_i = 0; index_i < length; index_i++) {
				if (!((cardData->cardHolderName[index_i] >= 'a' && cardData->cardHolderName[index_i] <= 'z')
					|| (cardData->cardHolderName[index_i] >= 'A' && cardData->cardHolderName[index_i] <= 'Z')
					|| (cardData->cardHolderName[index_i] == ' '))) {
					
					return WRONG_NAME;
				}
			}

		}
		printf("Hello MR,%s \n", cardData->cardHolderName);
		return CARD_OK;

	}
	else {
		return INVALID_ADDRESS;
	}	
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData) {

	if ( cardData != NULL ) {

		printf("Please enter the card expiration date in format (MM/YY) : ");
		gets((cardData->cardExpirationDate));
		uint8_t length = strlen(cardData->cardExpirationDate);

		if ( (cardData->cardExpirationDate[0] == '\0') || (length != 5) || (cardData->cardExpirationDate[2] != '/')
				||	(cardData->cardExpirationDate[0] < '0' || cardData->cardExpirationDate[0] > '1')) {

			return WRONG_EXP_DATE;
		}
		else if((cardData->cardExpirationDate[0] == '0') && !(cardData->cardExpirationDate[1] >= '1' && cardData->cardExpirationDate[1] <= '9')) {

			return WRONG_EXP_DATE;
		}
		else if((cardData->cardExpirationDate[0] == '1') && !(cardData->cardExpirationDate[1] >= '0' && cardData->cardExpirationDate[1] <= '2')){

			return WRONG_EXP_DATE;
		}
		else if ((cardData->cardExpirationDate[3] != '2') || (cardData->cardExpirationDate[4] > '4')) {

			return WRONG_EXP_DATE;

		}
		return CARD_OK;
	}
	else {
		return INVALID_ADDRESS;
	}
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData) {

	if (cardData != NULL) {
		printf("Please enter your PAN number which consists of max 19 digits : ");
		gets((cardData->primaryAccountNumber));
		uint8_t length = strlen(cardData->primaryAccountNumber);
		if (cardData->primaryAccountNumber == '\0' || ((length < 16) || (length > 19))) {
			return WRONG_PAN;
		}
		else {
			for (int index_i = 0; index_i < length; index_i++) {
				if (!(cardData->primaryAccountNumber[index_i] >= '0' && cardData->primaryAccountNumber[index_i] <= '9')) {
					return WRONG_PAN;
				}
			}		
		}
		return CARD_OK;
	}
	else {

		return INVALID_ADDRESS;
	}
}

int printclientdetails(ST_cardData_t* cardData) {

	if (cardData != NULL) {

		printf("The Client's Name is : %s \n", cardData->cardHolderName);
		printf("The Client's card expiration date is : %s \n", cardData->cardExpirationDate);
		printf("The Client's PAN number is : %s \n", cardData->primaryAccountNumber);
		return 0;
	}
	else {
		return 1;
	}
}