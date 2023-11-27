#include <stdio.h>
#include <stdlib.h>

#include "app.h"
#include "server.h"


extern ST_accountsDB_t accountsDB[255];
extern ST_transaction transactions[255];
extern EN_transStat_t Enum;
ST_transaction* transData = NULL;

void appStart(void) {
	
	Enum = recieveTransactionData(transData);
	saveTransaction(transData);

}

int main() {

	/*------------------------------------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------------------------------------*/
	accountsDB[0].balance = 100.0;
	strcpy(accountsDB[0].primaryAccountNumber, "123456789159357214");
	accountsDB[0].state = RUNNING;

	accountsDB[1].balance = 2000.0;
	strcpy(accountsDB[1].primaryAccountNumber, "123456789159357215");
	accountsDB[1].state = BLOCKED;

	accountsDB[2].balance = 3000.0;
	strcpy(accountsDB[2].primaryAccountNumber, "123456789159357216");
	accountsDB[2].state = RUNNING;

	accountsDB[3].balance = 4000.0;
	strcpy(accountsDB[3].primaryAccountNumber, "123456789159357217");
	accountsDB[3].state = BLOCKED;

	accountsDB[4].balance = 5000.0;
	strcpy(accountsDB[4].primaryAccountNumber, "123456789159357218");
	accountsDB[4].state = RUNNING;
	/*------------------------------------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------------------------------------*/
	printdata_accounts(accountsDB, 5);
	/*------------------------------------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------------------------------------*/

	for (unsigned char index_i = 0; index_i < 2; index_i++) {
		transData = &(transactions[index_i]);
		appStart();
	}
	/*------------------------------------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------------------------------------*/

	/*------------------------------------------------------------------------------------------------------*/
	listSavedTransactions();
	/*------------------------------------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------------------------------------*/
	printdata_accounts(accountsDB, 5);
	/*------------------------------------------------------------------------------------------------------*/
	/*------------------------------------------------------------------------------------------------------*/

}



