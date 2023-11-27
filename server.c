#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"

ST_accountsDB_t accountsDB[255] = {0};

ST_transaction transactions[255] = {0};

int status;

ST_accountsDB_t *Found_Account = NULL;

unsigned char state_word[] = "";

char *word = "";

unsigned int sequence_number = 0;

EN_transStat_t Enum = APPROVED;



void printdata_accounts(ST_accountsDB_t *accountsDB, unsigned char arr_size) {

	unsigned char index = 0;

	for (index = 0; index < arr_size; index++) {
		printf("balance for Client number %i : %0.2f\n", index,(accountsDB + index)->balance);
		printf("the state of the account for Client number %i : %i \n", index, (accountsDB + index)->state);
		printf("the PAN for Client number %i : %s\n", index, (accountsDB + index)->primaryAccountNumber);
		printf("-------------------------------------------------------------------------------------------\n");
	}
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence) {

	if (cardData != NULL && accountRefrence != NULL) {
			//TO DO :											/*Sorting*/
		unsigned char index = 0;
		for (index = 0; index < 5; index++) {			// TO DO : 5 to be changed according to array size

			if (strcmp(cardData->primaryAccountNumber,(accountRefrence + index)->primaryAccountNumber) == 0) {

				Found_Account = (accountRefrence + index);
				return SERVER_OK;
			}
		}	
		accountRefrence = NULL;
		return ACCOUNT_NOT_FOUND;
	}
	else {
		return INVALID_ADDRESS1;
	}

}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence) {

	if (accountRefrence != NULL) {

		if (accountRefrence->state == 0) {
			return SERVER_OK;
		}
		else {
			return BLOCKED_ACCOUNT;
		}
	}
	else {
		return INVALID_ADDRESS1;
	}
	

}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence) {
	if (termData != NULL && accountRefrence != NULL) {

		if (termData->transAmount > accountRefrence->balance) {
			return LOW_BALANCE;
		}
		return SERVER_OK;

	}
	else {
		return INVALID_ADDRESS1;
	}

}

EN_transStat_t recieveTransactionData(ST_transaction* transData) {

	/*--------------------------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------------------------*/
																								/*CARD DETAILS*/ /*START*/
	while (1) {

		status = getCardHolderName(&(transData->cardHolderData));
		if (status == WRONG_NAME || status == INVALID_ADDRESS) {

			printf("WRONG_NAME, Please Enter Another Name \n");
		}
		else {
			break;
		}
	}
	
	while (1) {

		status = getCardExpiryDate(&(transData->cardHolderData));
		if (status == WRONG_EXP_DATE || status == INVALID_ADDRESS) {
			printf("WRONG_EXP_DATE, Please Enter Valid Date Format \n");
		}
		else {
			break;
		}
	}

	while (1) {

		status = getCardPAN(&(transData->cardHolderData));
		if (status == WRONG_PAN || status == INVALID_ADDRESS) {
			printf("WRONG_PAN, Please Enter Valid PAN \n");
		}
		else {
			break;
		}
	}

	if (status == CARD_OK) {
		printf("CARD_OK \n");
		}

																								/*CARD DETAILS*/ /*END*/
	/*--------------------------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------------------------*/

	
	/*--------------------------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------------------------*/	  
																								/*TERMINAL DETAILS*/ /*START*/
	while (1) {
		status = getTransactionDate(&(transData->terminalData));
		if (status == WRONG_DATE || status == INVALID_ADDRESSw) {
			printf("WRONG_DATE, Please Enter Valid Date \n");
		}
		else {
			break;
		}
	}

	while (1) {
		status = isCardExpired(&(transData->cardHolderData), &(transData->terminalData));
		if (status == EXPIRED_CARD || status == INVALID_ADDRESSw) {
			printf("EXPIRED_CARD, Please Check Expiry Date \n");
			return FRAUD_CARD;
		}
		else {
			break;
		}
	}

	while (1) {
		status = getTransactionAmount(&(transData->terminalData));
		if (status == INVALID_AMOUNT || status == INVALID_ADDRESSw) {
			printf("INVALID_AMOUNT, Please Enter Valid Amount \n");
		}
		else {
			break;
		}

	}
	while (1) {

		status = setMaxAmount(&(transData->terminalData), 1000.0);
		if (status == INVALID_MAX_AMOUNT || status == INVALID_ADDRESSw) {
			printf("INVALID_MAX_AMOUNT, Please Enter Valid max Amount \n");
		}
		else {
			break;
		}
	}

	while (1) {

		status = isBelowMaxAmount(&(transData->terminalData));
		if (status == EXCEED_MAX_AMOUNT || status == INVALID_ADDRESSw) {
			printf("EXCEED_MAX_AMOUNT, Please Enter Valid amount next time \n");
			return EXCEED_MAX_AMOUNT;
		}
		else {
			break;
		}
	}
	if (status == TERMINAL_OK) {
		printf("TERMINAL_OK \n");
	}																									
																									/*TERMINAL DETAILS*/ /*END*/
	/*--------------------------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------------------------*/
																										
	/*--------------------------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------------------------*/
																									/*SERVER DETAILS*/ /*START*/
	status = isValidAccount(&(transData->cardHolderData), accountsDB);
	if (status == ACCOUNT_NOT_FOUND || status == INVALID_ADDRESS1) {
		printf("ACCOUNT_NOT_FOUND \n");
		return FRAUD_CARD;							//account does not exist
	}
	status = isBlockedAccount(Found_Account);
	if (status == BLOCKED_ACCOUNT || status == INVALID_ADDRESS1) {
		printf("BLOCKED_ACCOUNT \n");
		return DECLINED_STOLEN_CARD;
	}
	status = isAmountAvailable(&(transData->terminalData), Found_Account);
	if (status == LOW_BALANCE || status == INVALID_ADDRESS1) {
		printf("LOW_BALANCE \n");
		return DECLINED_INSUFFECIENT_FUND;
	}
	else if (status == SERVER_OK) {
		printf("SERVER_OK \n");
		return APPROVED;
	}
	else {
		
		return INTERNAL_SERVER_ERROR;
	}
																										/*SERVER DETAILS*/ /*END*/
	/*--------------------------------------------------------------------------------------------*/
	/*--------------------------------------------------------------------------------------------*/

}

EN_serverError_t saveTransaction(ST_transaction* transData) {

	if (transData != NULL) {
		if (Enum == FRAUD_CARD) {
			transData->transState = FRAUD_CARD;
		}
		else if (Enum == DECLINED_STOLEN_CARD) {
			transData->transState = DECLINED_STOLEN_CARD;
		}
		else if (Enum == DECLINED_INSUFFECIENT_FUND) {
			transData->transState = DECLINED_INSUFFECIENT_FUND;
		}
		else if (Enum == EXCEED_MAX_AMOUNT) {
			transData->transState = EXCEED_MAX_AMOUNT;
		}
		else if (Enum == APPROVED) {
			Found_Account->balance = (Found_Account->balance) - (transData->terminalData.transAmount);
			transData->transState = APPROVED;
		}
		else {
			transData->transState = INTERNAL_SERVER_ERROR;
		}
		transData->transactionSequenceNumber = sequence_number++;
	}

}

void listSavedTransactions(void) {

	unsigned  char index_i = 0;
	

	for (index_i = 0; index_i < 5; index_i++) {

		unsigned char trans_state = transactions[index_i].transState;
		if (trans_state == 0) {
			word = "Approved";
		}
		else if (trans_state == 1) {
			word = "DECLINED_INSUFFECIENT_FUND";
		}
		else if (trans_state == 2) {
			word = "DECLINED_STOLEN_CARD";
		}
		else if (trans_state == 3) {

			word = "FRAUD_CARD";
		}
		else if (trans_state == 4) {

			word = "INTERNAL_SERVER_ERROR";

		}
		else if (trans_state == 5) {

			word = "EXCEED_MAX_AMOUNT";

		}
		printf("The Transaction State is : %s \n", word);
		printf("The Name is : %s \n", transactions[index_i].cardHolderData.cardHolderName);
		printf("The PAN is : %s \n", transactions[index_i].cardHolderData.primaryAccountNumber);
		printf("The Expiration Date  is : %s \n", transactions[index_i].cardHolderData.cardExpirationDate);
		printf("The Transaction date is : %s \n", transactions[index_i].terminalData.transactionDate);
		printf("The max Transaction amount is : %f \n", transactions[index_i].terminalData.maxTransAmount);
		printf("The Transaction amount is : %f \n", transactions[index_i].terminalData.transAmount);
		printf("The Sequence number is : %i \n", transactions[index_i].transactionSequenceNumber);
		printf("---------------------------------------------------------------------------------------------------- \n");

	}
}


