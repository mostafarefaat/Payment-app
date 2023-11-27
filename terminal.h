#ifndef terminal_h
#define terminal_h
#define _CRT_SECURE_NO_WARNINGS

#include <time.h>
#include "card.h"


typedef struct ST_terminalData_t
{
    float transAmount;
    float maxTransAmount;
    uint8_t transactionDate[11];
}ST_terminalData_t;

typedef enum EN_terminalError_t
{
    TERMINAL_OK, WRONG_DATE, EXPIRED_CARD, INVALID_CARD, INVALID_AMOUNT, EXCEED_MAX_AMOUNT, INVALID_MAX_AMOUNT, INVALID_ADDRESSw
}EN_terminalError_t;


struct tm date;


EN_terminalError_t getTransactionDate(ST_terminalData_t* termData);
EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData);
EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData);
EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData);
EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount);
int printterminaldetails(ST_terminalData_t* termData);
EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData); // Optional

#endif // !terminal.h
