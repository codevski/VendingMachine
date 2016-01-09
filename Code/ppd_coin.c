/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Semester 2 2015 Assignment #2
 * Full Name        : Saso Petrovski
 * Student Number   : S3435996
 * Course Code      : COSC1076
 * Program Code     : BP096
 * Start up code provided by Paul Miller
 * Some codes are adopted here with permission by an anonymous author
 ***********************************************************************/
#include "ppd_coin.h"
#include "ppd_utility.h"

 /**
  * @file ppd_coin.c implement functions here for managing coins in the
  * "cash register" contained in the @ref ppd_system struct.
  **/
BOOLEAN load_coins(struct ppd_system * system , char * buffer)
{
	static int coin_num = NUM_DENOMS-1;
	static int counter = 0;
	char *token, *cptr, *cents, *hand;
	int denompos = 0;
	token = strtok(buffer, COIN_DELIM);

	while (token != NULL)
	{
		cents = token;
		hand = strtok(NULL, ",\n");
		/*
		 * Check that after tokenising that the cents and hands are valid
		 */
		if((denompos = validate_coin(system, cents, hand, coin_num, counter))
				== FALSE)
		{
			return FALSE;
		}
		/* Reverting to original position this allows
		 * coin file to be dynamic and in any order up to 8 lines
		 */
		--denompos;

		/*
		 * Adding to struct
		 */
		system->cash_register[denompos].denom = strtol(cents, &cptr, 10);
		system->cash_register[denompos].count = strtol(hand, &cptr, 10);
		--coin_num;
		counter++;
		token = strtok(NULL, COIN_DELIM);
	}
	return TRUE;
}

BOOLEAN check_till(struct ppd_system * system, struct bank * bank, int change)
{

	/*
	 * Function is used to make sure that there are coins in the till
	 * so that we can return change. If there are no coins in the till we
	 * would return false
	 */
	int i, available = 0;

	for(i = 0; i < 9; i++)
	{
		available += system->cash_register[i].denom *
				system->cash_register[i].count;
	}
	if(change > available)
	{
		return FALSE;
	}

	return TRUE;
}
BOOLEAN update_till(struct ppd_system * system, struct bank * bank, int cents)
{
	/*
	 * Function is used to finally update the list cash register. It uses the
	 * temp bank storage incase they decide to cancel the purchase while they
	 * added some money into the Vending Machine
	 */
	int i, j, dollar=0;

	for(i = 0; i <= bank->current; i++)
	{
		for(j=0; j <= 8; j++)
		{
			if(bank->money[i] == system->cash_register[j].denom)
			{
				/* Updating list cash register */
				system->cash_register[j].count++;
			}
		}
	}
	i = 8;
	do
	{
		if(system->cash_register[i].denom <= cents  &&
				system->cash_register[i].count != 0)
		{
			/* Display the change, either cents or dollars*/
			if(system->cash_register[i].denom < 100)
			{
				printf("%dc ", format_cents(dollar,system->cash_register[i].denom));
				cents = cents - system->cash_register[i].denom;
				system->cash_register[i].count--;
			}
			else
			{
				printf("$%d ", format_dollars(system->cash_register[i].denom));
				cents = cents - system->cash_register[i].denom;
				system->cash_register[i].count--;
			}
		}
		else
		{
			i--;
		}
	}while(cents != 0 && i < 8);

	return FALSE;
}

int format_dollars(int cents)
{
	/*
	 * Function to be called to format the dollars when needed
	 */
	return (float)cents / 100;
}

BOOLEAN write_coins(struct ppd_system * system, FILE * coinFile)
{
	/*
	 * Function used to write coins to the coin file.
	 */
	int i;

	for(i = NUM_DENOMS-1; i >= 0; i--)
	{
		fprintf(coinFile, "%u,%u\n", system->cash_register[i].denom, system->cash_register[i].count);
	}
	return TRUE;
}

BOOLEAN validate_coin(struct ppd_system * system, char * cents, char * hand, int coin_num, int counter)
{
	/*
	 * Function used to validate the imported coin file.
	 */
	int centlen = 0, handlen = 0, i, centsint, handint, denompos;
	char * cptr;

	/* Check if cents or hand is NULL before testing them */
	if(cents == NULL)
	{
		return FALSE;
	}

	if(hand == NULL)
	{
		return FALSE;
	}
	centlen = strlen(cents);
	handlen = strlen(hand);

	if(counter > NUM_DENOMS-1)
	{
		fprintf(stderr, "Too many unique coins in your file, Please check your "
				"file and try again\n");
		return FALSE;
	}
	/* Cents validation */
	/* Make sure that total length isnt past 4 e.g 1000 = $10*/
	if(strlen(cents) > 4)
	{
		fprintf(stderr, "Invalid Coins: Cent length of > 4, Please check your "
				"coin file and try again\n");
		return FALSE;
	}
	/* Check if its a digit */
	for(i = 0; i < centlen; i++)
	{
		if(!isdigit(cents[i]))
		{
			fprintf(stderr, "Invalid Coins: You have characters in your coin "
					"file, Please check your coin file and try again\n");
			return FALSE;
		}
	}

	/* on Hand validation */
	if(strlen(hand) > 2)
	{
		fprintf(stderr, "Invalid Coins: On hand length is > 2 (Max: 99), "
				"Please check your coin file and try again\n");
		return FALSE;
	}
	/* Check if its a digit */
	for(i = 0; i < handlen; i++)
	{
		if(!isdigit(hand[i]))
		{
			fprintf(stderr, "Invalid Coins: You have characters in your on "
					"hand, Please check your coin file and try again\n");
			return FALSE;
		}
	}
	centsint = strtol(cents, &cptr, 10);
	handint = strtol(hand, &cptr, 10);
	if(handint > 99)
	{
		fprintf(stderr, "Invalid Coins: On hand is greater than 99, Please "
				"check your coin file and try again\n");
		return FALSE;
	}

	/*
	 * Make sure the correct coin is associated with denom from $10 down to 5c
	 * making this a little more dynamic allowing the coin file to be in any
	 * order.
	 */
	if(centsint == 1000)
	{
		denompos = TEN_DOLLARS;

	}
	else if(centsint == 500)
	{
		denompos = FIVE_DOLLARS;

	}
	else if(centsint == 200)
	{
		denompos = TWO_DOLLARS;

	}
	else if(centsint == 100)
	{
		denompos = ONE_DOLLAR;

	}
	else if(centsint == 50)
	{
		denompos = FIFTY_CENTS;

	}
	else if(centsint == 20)
	{
		denompos = TWENTY_CENTS;

	}
	else if(centsint == 10)
	{
		denompos = TEN_CENTS;

	}
	else if(centsint == 5)
	{
		denompos = FIVE_CENTS;

	}
	else
	{
		fprintf(stderr, "Invalid Coins: We only accept 1000, 500, 200, 100 "
				"50, 20, 10, 5, Please check your coin file and try again\n");
		return FALSE;
	}
	denompos++;
	return denompos;
}
