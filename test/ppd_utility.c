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

#include "ppd_utility.h"
#define DELIM ",\t\r\n"
#define GETINTLEN 1
/**
 * @file ppd_utility.c contains implementations of important functions for
 * the system. If you are not sure where to implement a new function, 
 * here is probably a good spot.
 **/

void read_rest_of_line(void)
{
    int ch;
    /* keep retrieving characters from stdin until we
     * are at the end of the buffer
     */
    while(ch = getc(stdin), ch!='\n' && ch != EOF)
        ;
    /* reset error flags on stdin */
    clearerr(stdin);
}

/**
 * @param system a pointer to a @ref ppd_system struct that contains
 * all the data in the system we are manipulating
 * @param coins_name the name of the coins file
 * @param stock name the name of the stock file
 **/
BOOLEAN load_data(struct ppd_system * system , const char * coins_name, 
const char * stock_name)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    char items[MAX_STOCK_LINE];
    char coins[MAX_COIN_LINE];
    int countstock = 0, countcoin = 0;
    BOOLEAN condition;

	FILE *fpcoins, *fpstock;

	/* Open the files */
	/* If there is a problem with opening the files */
	if((fpstock = fopen(stock_name, "r")) == NULL)
    {
    	fprintf(stderr, "Cannot open the Stock File. "
    			"Please check the file and try again\n");
    	return FALSE;
    }

	if((fpcoins = fopen(coins_name, "r")) == NULL)
	    {
	    	fprintf(stderr, "Cannot open the Coin File. "
	    			"Please check the file and try again\n");
	    	return FALSE;
	    }

	/* Start reading each line and checking that the condition are fine */
	while(fgets(coins, sizeof(coins), fpcoins) != NULL)
	{
		if(coins[0] != '\n' )
		{
			condition = load_coins(system, coins);
			countcoin++;
		}

		if(condition == FALSE)
		{
			fclose(fpcoins);
			fclose(fpstock);
			return FALSE;
		}
	}

	while(fgets(items, sizeof(items), fpstock) != NULL)
	{
		if(items[0] != '\n' )
		{
			condition = load_stock(system, items);
			countstock++;
		}

		if(condition == FALSE)
		{
			printf("ERROR\n");
			fclose(fpcoins);
			fclose(fpstock);
			return FALSE;
		}
	}

	/*
	 * If the vending machine has more than 30 items or not exactly 8 different
	 * coins return an error.
	 */
	if(countstock > 30 || countcoin < 8)
	{
		printf("Please check you Stock and Coin file\n");
		printf("This vending machine only holds a total of 30 different");
		printf(" items and accepts exact 8 different coins\n");
		fclose(fpcoins);
		fclose(fpstock);
		return FALSE;
	}

	fclose(fpcoins);
	fclose(fpstock);
    return TRUE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
BOOLEAN system_init(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    system->item_list = create_list();
    return TRUE;
}

/**
 * @param system a pointer to a @ref ppd_system struct that holds all 
 * the data for the system we are creating
 **/
void system_free(struct ppd_system * system)
{
	/*
	 * Free all the data in the list
	 */
	struct ppd_node *curr_node;

	while((curr_node = system->item_list->head) != NULL)
	{
		system->item_list->head = system->item_list->head->next;
		free(curr_node->data);
		free(curr_node);
	}
	free(system->item_list->head);
	free(system->item_list);
}

/* Steven Borrow */
int getString(char* string, unsigned LENGTH)
{
   int finished = FALSE;
   char *tempString = NULL;

   tempString = malloc((LENGTH + 2)*((int) sizeof(char))
                  + 2);

   /* Continue to interact with the user until the input is valid. */
   do
   {
      /* Accept input. "+2" is for the \n and \0 characters. */
      if(fgets(tempString, LENGTH + 2, stdin) == NULL)
      {
    	  free(tempString);
    	  return RTM;
      }
      else
      {
    	  if(tempString[0] == '\n')
    	  {
    		  free(tempString);
    		  return RTM;
    	  }
      }

      /* A string that doesn't have a newline character is too long. */
      if (tempString[strlen(tempString) - 1] != '\n')
      {
    	  printf("Input was too long. Please try again!\n");
    	  read_rest_of_line();
      }
      else
      {
         finished = TRUE;
      }
   } while (finished == FALSE);

   /* Overwrite the \n character with \0. */
   tempString[strlen(tempString) - 1] = '\0';

   /* Make the result string available to calling function. */
   strcpy(string, tempString);
   free(tempString);

   return TRUE;
}

/* Steven Borrow */
int getInteger(int* integer, unsigned length, char* prompt, int min, int max)
{
	int finished = FALSE;
	char tempString[GETINTLEN + 2];
	int tempInteger = 0;
	char* endPtr;

	do
	{
		printf("%s", prompt);

		/* check if the user ctrl+D */
		if(fgets(tempString, length + 2, stdin) == NULL)
		{
			printf("\nReturn to menu");
			return RTM;
		}
		else
			if(tempString[0] == '\n')
			{
				printf("Return to Menu\n");
				return RTM;
			}

		if (tempString[strlen(tempString) - 1] != '\n')
		{
			printf("Input was too long.\n");
			read_rest_of_line();
		}
		else
		{
			tempString[strlen(tempString) - 1] = '\0';
			tempInteger = (int) strtol(tempString, &endPtr, 10);

			if (strcmp(endPtr, "") != 0)
			{
				printf("Input was not numeric.\n");
			}
			else if (tempInteger < min || tempInteger > max)
			{
				printf("Input was not within range %d - %d.\n", min, max);
			}
			else
			{
				finished = TRUE;
			}
		}
	} while (finished == FALSE);

	*integer = tempInteger;
	return TRUE;
}
struct ppd_node *search_node(struct ppd_system * system, char *input)
{
	/*
	 * Function that is used to search the list and returns the node if
	 * the item was found. If no item is found returns NULL
	 */
	struct ppd_node *curr_node;
	curr_node = system->item_list->head;
	while(curr_node != NULL)
	{
		if(strcmp(input, curr_node->data->id) == 0)
		{
			return curr_node;
		}
		else
		{
			curr_node = curr_node->next;
		}
	}

	return NULL;
}
BOOLEAN valid_price(struct ppd_system * system, char *dollar, char *cent)
{
	/*
	 * Function that validates the dollar and cents
	 */
	if(validate_stock_coins(system, dollar, cent) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

int format_cents(int dollar, int cents)
{
	/*
	 * Simple function that just returns cents format of a price.
	 */
	return (dollar * 100) + cents;
}

int valid_coin(struct ppd_system * system, char * input)
{
	/*
	 * Checks if a coin is valid. That way the user cannot input a coin that
	 * doesn't exist or is not valid to the vending machine.
	 */
	int i, cents;
	char *cptr;
	cents = strtol(input, &cptr, 10);

	for(i=0; i < 8; i++)
	{
		if(cents == system->cash_register[i].denom)
		{
			return TRUE;
		}
	}
	return FALSE;
}

int moneyCache(int input, int limit, struct bank * cache)
{
	/*
	 * Function that acts as a cache of input. Here is where my limit is
	 * checked that the user cannot input more than 10 consecutive coins at
	 * one given transaction.
	 */
	if(limit > 10)
	{
		return FALSE;
	}
	cache->money[limit] = input;
	cache->current = limit;

	return TRUE;
}

BOOLEAN processing(struct ppd_system * system, struct ppd_node * tempNode)
{
	/*
	 * Function that does the main processing. This can also be pictured as
	 * the check out clerk.
	 */
	struct bank bank;
	char coins[10], *cptr;
	int input, cents=0, itemPrice, limit = 0, total=0;
	BOOLEAN correntMoney = FALSE;

	itemPrice = format_cents(tempNode->data->price.dollars,
			tempNode->data->price.cents);

	/* Keep trying till the user input's the correct money */
	correntMoney = FALSE;
	while(!correntMoney)
	{
		if(getString(coins, 10) == RTM)
		{
			return RTM;
		}
		while(valid_coin(system, coins) == FALSE)
		{
			fprintf(stderr, "Invalid Coin!\n");
			fprintf(stderr, "Please enter again:\n");
			if(getString(coins, 10) == RTM)
			{
				return RTM;
			}
		}

		input = strtol(coins, &cptr, 10);
		if(moneyCache(input, limit, &bank) == FALSE)
		{
			fprintf(stderr, "Error: More than 10 coins. Transaction Cancelled"
					"Returning change to user. Please try again.\n");
			return FALSE;
		}
		total += input;

		if(itemPrice != total && total < itemPrice)
		{
			cents = itemPrice - total;
			printf("You still need to give us %d: \n", cents);
		}
		else if(total > itemPrice)
		{
			cents = total - itemPrice;
			if(check_till(system, &bank, cents) == FALSE)
			{
				fprintf(stderr, "Error: Not enough money in the till. "
						"Returning money to user.\n");
				return FALSE;
			}

			printf("Thank you. Here is your %s, ", tempNode->data->name);
			printf("and your change of $%.2f: ", cents_to_dollars(cents));
			update_till(system, &bank, cents);

			correntMoney = TRUE;
		}
		else
		{
			printf("Thank you. Here is your %s, no change given\n",
					tempNode->data->name);
			cents = total - itemPrice;
			update_till(system, &bank, cents);
			correntMoney = TRUE;
		}
		limit++;
	}
	printf("\nPlease come again soon.\n");
	return TRUE;
}

float cents_to_dollars(int cents)
{
	return ((float)cents/100);
}
