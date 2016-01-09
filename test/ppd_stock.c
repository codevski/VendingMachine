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
#include "ppd_stock.h"

 /**
  * @file ppd_stock.c this is the file where you will implement the 
  * interface functions for managing the stock list.
  **/
BOOLEAN load_stock(struct ppd_system * system, char * buffer)
{
	struct ppd_stock * tempNode;
	char *token, *cptr, *dollar, *cent;
	char * stock_array[5];
	int i = 0;

	if((tempNode = malloc(sizeof(*tempNode))) == NULL)
	{
		fprintf(stderr, "Error: Memory allocation failed\n" );
		return FALSE;
	}

	token = strtok(buffer, STOCK_DELIM);

	while(token != NULL)
	{
		stock_array[i] = token;		
		token = strtok(NULL, STOCK_DELIM);
		i++;
	}
	
	/* Check if there are too many deliminators */
	if(i > 5)
	{
		printf("Error t\n");
		free(tempNode);
		return FALSE;
	}
	
	/* Check if the stock are valid not including the coin field */
	if(validate_stock(system, stock_array) == FALSE)
	{
		free(tempNode);
		return FALSE;
	}
	
	dollar = strtok(stock_array[3], ".");
	cent = strtok(NULL, ".");

	/* Check if the coin field are valid */
	if(validate_stock_coins(system, dollar, cent) == FALSE)
	{
		free(tempNode);
		return FALSE;
	}
	
	strcpy(tempNode->id, stock_array[0]);
	strcpy(tempNode->name, stock_array[1]);
	strcpy(tempNode->desc, stock_array[2]);
	tempNode->price.dollars = strtol(dollar, &cptr, 10);
	tempNode->price.cents = strtol(cent, &cptr, 10);
	tempNode->on_hand = strtol(stock_array[4], &cptr, 10);
	
	/* When all validation have been checked continue to add to the list */
	add_node(system, tempNode);

	return TRUE;
}

struct ppd_list * create_list()
{
	struct ppd_list * list = malloc(sizeof(*list));

	assert(list);

	list->head = NULL;
	list->count = 0;

	return list;

}

BOOLEAN add_node(struct ppd_system * system, struct ppd_stock * tempNode)
{
	struct ppd_node *newNode, *prev, *curr;

	if((newNode = malloc(sizeof(*newNode))) == NULL)
	{
		fprintf(stderr, "Error: Memory allocation failed\n" );
		return FALSE;
	}

	newNode->data = tempNode;
	newNode->next = NULL;

	prev = NULL;
	curr = system->item_list->head;

	/* Using insertion algorithm to find a spot to add the new node in */
	while(curr != NULL)
	{
		if(strcmp(newNode->data->name, curr->data->name) < 0)
		{
			break;
		}
		prev = curr;
		curr = curr->next;
	}

	if(prev == NULL)
	{
		newNode->next = system->item_list->head;
		system->item_list->head = newNode;
	}
	else
	{
		prev->next = newNode;
		newNode->next = curr;
	}

	system->item_list->count++;
	return FALSE;
}

BOOLEAN write_stock(struct ppd_system * system, FILE * stockFile)
{
	struct ppd_node *curr_node;

	curr_node = system->item_list->head;

	/* Check if list has items */
	if(curr_node->data == NULL)
	{
		fprintf(stderr, "Error: Cannot write stock. There are no items\n");
		return FALSE;
	}

	while(curr_node != NULL)
	{
		fprintf(stockFile, "%s|%s|%s|%u.%02u|%u\n",
				curr_node->data->id, curr_node->data->name,
				curr_node->data->desc, curr_node->data->price.dollars,
				curr_node->data->price.cents, curr_node->data->on_hand);

		curr_node = curr_node->next;
	}
	return TRUE;
}

BOOLEAN validate_stock(struct ppd_system * system, char * stock_items[])
{
	int i = 0, len = 0, onhandint;
	char *cptr;

	/* Check that all fields are in set and ready for validation */
	for(i = 0; i < 5; i++)
	{
		if(stock_items[i] == NULL)
		{
			fprintf(stderr, "Invalid Stock: There is a problem with one of the "
					"lines. Please check your stock file and try again. ");
			return FALSE;
		}
	}

	/* Test Empty File */
	if(strlen(stock_items[0]) == 0)
	{
		fprintf(stderr, "Invalid Stock: There is a problem with one of the "
							"lines. Please check your stock file and "
							"try again.\n");
		return FALSE;
	}

	/*
	 * Test ID block
	 * Check if its an I at the beginning
	 */
	if(isalpha(stock_items[0][0]) != 0)
	{
		if(stock_items[0][0] != 'I')
		{
			fprintf(stderr, "Invalid Stock: Stock does not start with \"I\" "
					"Vending Machine needs a service\n");
			return FALSE;
		}
	}
	else
	{
		fprintf(stderr, "Invalid Stock: Stock does not start with an alphabet. "
							"Vending Machine needs a service\n");
		return FALSE;
	}

	/* Test Buffer Length */
	/* For loop isdigit starting i from 1 */
	for(i = 1; i < 5; i++)
	{
		if(!isdigit(stock_items[0][i]))
		{
			fprintf(stderr, "Invalid Stock: ID needs to have 4 digits after "
					"an alphabet\n");
			return FALSE;
		}
	}

	/* Test length of block ID */
	if(strlen(stock_items[0]) != 5)
	{
		fprintf(stderr, "Invalid Stock: ID needs to be length of 5\n");
		return FALSE;
	}

	/* Searching the list if the stock already exists */
	if(search_node(system, stock_items[0]) != NULL)
	{
		fprintf(stderr, "Invalid Stock: Duplication Detected. Please check the "
				"stock file and try again.\n");
		return FALSE;
	}

	/* Test NAME block */
	if(strlen(stock_items[1]) > NAMELEN + 1)
	{
		fprintf(stderr, "Invalid Stock: Stock name is too long. Please check "
				"the stock file and try again.\n");
		return FALSE;
	}

	/* Test DESC block */
	if(strlen(stock_items[2]) > DESCLEN)
	{
		fprintf(stderr, "Invalid Stock: Stock description is too long. "
				"Please check the stock file and try again.\n");
		return FALSE;
	}

	/* Test HAND block */
	/* Test if on hand is more than 3 in length */
	if(strlen(stock_items[4]) > 3)
	{
		fprintf(stderr, "Invalid Stock: On Hand Error. Please check the stock "
				"file and try again\n");
		return FALSE;
	}
	/* Validate isdigit */
	if(strlen(stock_items[4]) == 1)
	{
		/* Dollar: Check if its a digit 1/2 */
		if(!isdigit(stock_items[4][1]))
		{
			fprintf(stderr, "Invalid Stock: On hand needs to be a digit. "
					"Please check the stock file and try again\n");
			return FALSE;
		}
	}
	else if(strlen(stock_items[4]) == 2)
	{
		for(i = 0; i < len; i++)
		{
			/* Dollar: Check if its a digit */
			if(!isdigit(stock_items[4][i]))
			{
				fprintf(stderr, "Invalid Stock: On hand needs to be a digit. "
									"Please check the stock file and "
									"try again\n");
				return FALSE;
			}
		}
	}
	/* Test less than 99 items */
	onhandint = strtol(stock_items[4], &cptr, 10);
	if(onhandint > 99)
	{
		fprintf(stderr, "Invalid Stock: On hand needs to be less than 99 "
				"(Max: 99. Please check the stock file and try again.\n");
		return FALSE;
	}
	return TRUE;
}

BOOLEAN validate_stock_coins(struct ppd_system * system, char * dollars,
		char * cents)
{
	/*
	 * Function validates if the currency in the stock file is in the correct
	 * format.
	 */
	int dollarint, centint;
	int len = 2, i = 0;
	char *cptr;

	/* If the dollar is between two digits or one */
	if(strlen(dollars) == 1)
	{
		/* Dollar: Check if its a digit 1/2 */
		if(!isdigit(dollars[0]))
		{
			fprintf(stderr, "Error: Dollar needs to be a digit.\n");
			return FALSE;
		}
	}
	else if(strlen(dollars) == 2)
	{
		for(i = 0; i < len; i++)
		{
			/* Dollar: Check if its a digit 2/2*/
			if(!isdigit(dollars[i]))
			{
				fprintf(stderr, "Error: Dollar needs to be a digit.\n");
				return FALSE;
			}
		}
	}

	/* Dollar: Check if value is less than 99 */
	dollarint = strtol(dollars, &cptr, 10);
	if(dollarint > 99 || dollarint < 0)
	{
		fprintf(stderr, "Error: Dollar needs to be less than $100.\n");
		return FALSE;
	}

	/* CENTS VALIDATION SECTION
	 *
	 */
	if(cents == NULL)
	{
		return FALSE;
	}
	/*cent: Check length of 2 */
	if(strlen(cents) != 2)
	{
		fprintf(stderr, "Error: Cents needs correct format two digits.\n");
		return FALSE;
	}
	/* Cent: Check if its a digit */
	if(!isdigit(cents[0]))
	{
		fprintf(stderr, "Error: Cents needs to be a digit.\n");
		return FALSE;
	}
	if(!isdigit(cents[1]))
	{
		fprintf(stderr, "Error: Cents needs to be a digit.\n");
		return FALSE;
	}

	/* Cent: Check if value is less than 99 */
	centint = strtol(cents, &cptr, 10);
	if(centint > 99 || centint < 0)
	{
		fprintf(stderr, "Error: Cents needs to be Between 00 - 99.\n");
		return FALSE;
	}

	/* Cent: Check if cent is modular 5*/
	if(centint % 5 != 0)
	{
		fprintf(stderr, "Error: Price is not modular of 5.\n");
		return FALSE;
	}
	return TRUE;
}
