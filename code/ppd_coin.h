#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include "ppd_stock.h"

/**
 * @file ppd_coin.h defines the coin structure for managing currency in the
 * system. You should declare function prototypes for managing coins here
 * and implement them in ppd_coin.c
 **/
#ifndef PPD_COIN
#define PPD_COIN
#include "ppd_main.h"
#define COIN_DELIM ","
struct ppd_system;

#define MAX_COIN_LINE 7
#define TEMP_COINS_LIMIT 10
#define MAX_COIN_LEN 5

/**
 * enumeration representing the various types of currency available in
 * the system. 
 **/
enum denomination
{
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR, 
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS
};

/**
 * represents a coin type stored in the cash register. Each demonination
 * will have exactly one of these in the cash register.
 **/
struct coin
{
    /**
     * the denomination type
     **/
    enum denomination denom;
    /**
     * the count of how many of these are in the cash register
     **/
    unsigned count;
};

/*
 * Temporary bank storage to hold the coins the users puts into the bank.
 */
struct bank
{
	unsigned money[TEMP_COINS_LIMIT];
	unsigned current;
};

BOOLEAN load_coins(struct ppd_system * system , char * buffer);
BOOLEAN check_till(struct ppd_system * system, struct bank * bank, int cents);
BOOLEAN update_till(struct ppd_system * system, struct bank * bank, int cents);
int format_dollars(int cents);
BOOLEAN write_coins(struct ppd_system * system, FILE * coinFile);
BOOLEAN validate_coin(struct ppd_system * system, char * cents, char * hand, int coin_num, int counter);

#endif
