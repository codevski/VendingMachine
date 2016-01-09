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

#include "ppd_options.h"

/**
 * @file ppd_options.c this is where you need to implement the main 
 * options for your program. You may however have the actual work done
 * in functions defined elsewhere. 
 * @note if there is an error you should handle it within the function
 * and not simply return FALSE unless it is a fatal error for the 
 * task at hand. You want people to use your software, afterall, and
 * badly behaving software doesn't get used.
 **/

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this particular function should never fail.
 **/
BOOLEAN display_items(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
	int i;
	struct ppd_node *curr_node;
	curr_node = system->item_list->head;
	printf("Items Menu\n");
	printf("----------\n");
	printf("%-*s| %-*s | %-7s | %s\n", IDLEN, "ID", NAMELEN, "NAME",
	                                             "AVAILABLE","PRICE");
	printf("------------------------------------------------------------"
			"---------\n");
	for(i = 0; i < system->item_list->count; i++)
	{
		printf("%*s| ", IDLEN, curr_node->data->id);
		printf("%-*s |", NAMELEN, curr_node->data->name);
		printf(" %-7d   |", curr_node->data->on_hand);
		printf("$ %-d.%.2d\n", curr_node->data->price.dollars,
				curr_node->data->price.cents);
		curr_node = curr_node->next;
	}
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a purchase succeeds and false when it does not
 **/
BOOLEAN purchase_item(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
	char id[IDLEN];
	struct ppd_node * tempNode;

	printf("Purchase Item\n");
	printf("-------------\n");
	printf("Please enter the id of the item you wish to purchase: \n");
	if(getString(id, IDLEN) == RTM)
	{
		return FALSE;
	}

	/* Check if the item exists */
	if((tempNode = search_node(system, id)) == NULL)
	{
		fprintf(stderr, "\nID Not found. Transaction Cancelled. "
				"Returning to menu\n");
		return FALSE;
	}

	/* Check if stock is available for the item */
	if(tempNode->data->on_hand == 0)
	{
		fprintf(stderr, "\nSorry item is out of stock. Transaction Cancelled. "
				"Returning to menu\n");
		return FALSE;
	}

	if(tempNode != NULL)
	{
		printf("You have selected \"%s - %s\". This will cost you %d.%.2d\n",
				tempNode->data->name,
				tempNode->data->desc,
				tempNode->data->price.dollars,
				tempNode->data->price.cents);
		printf("Please hand over the money - type in the value of each "
				"note/coin in cents.\n");
		printf("Press enter or ctrl-d on the new line to cancel this "
				"purchase: \n");

		/* Send the item to be processed */
		if(processing(system, tempNode) == TRUE)
		{
			tempNode->data->on_hand--;
		}

	}
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when a save succeeds and false when it does not
 **/
BOOLEAN save_system(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    FILE * stockFile;
    FILE * coinFile;

    printf("Save and Exit\n");

    /* Check if files are in working order */
    /* Check Stock file */
    if((stockFile = fopen(system->stock_file_name, "w")) == NULL)
    {
    	fprintf(stderr, "Cannot open the Stock File. "
    			"Please check the file and try again\n");
    	system_free(system);
    	return FALSE;
    }
    else
    {
    	write_stock(system, stockFile);
    	fclose(stockFile);
    }

    /* Check Coin File */
    if((coinFile = fopen(system->coin_file_name, "w")) == NULL)
    {
    	fprintf(stderr, "Cannot open the Coin File. "
    			"Please check the file and try again\n");
    	system_free(system);
    	return FALSE;
    }
    else
    {
    	write_coins(system, coinFile);
    	fclose(coinFile);
    }

    system_free(system);
    exit(0);
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when adding an item succeeds and false when it does not
 **/
BOOLEAN add_item(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
	struct ppd_stock * tempNode;
	char id[8], *dollar, *cent, *cptr;
	char name[NAMELEN], desc[DESCLEN], inputPrice[MAX_COIN_LEN];
	int unique = FALSE;
	int counter = 1;
	BOOLEAN valid;

    if((tempNode = malloc(sizeof(*tempNode))) == NULL)
    {
    	fprintf(stderr, "Error: Memory allocation failed\n" );
    	return FALSE;
    }

    /*
     * If there are 30 or more items don't allow anymore to be added. The max
     * items the Vending machine can hold is 30.
     */
	if(system->item_list->count >= 30)
	{
		printf("Too many times in the Vending Machine(Max 30), "
				"Please remove items before adding more");
		free(tempNode);
		return FALSE;
	}

	printf("This new meal item will have the Item id of: \n");

	/* The next available ID */
	sprintf(id, "I%.4d", counter);

	/* Here I will send I0001 to search and increment,
	 * if return NULL its a spare spot if return struct spot not available
	 */
	while(!unique)
	{
		if(search_node(system, id) != NULL)
		{
			sprintf(id, "I%.4d", ++counter);
		}
		else if (search_node(system, id) == NULL)
		{
			printf("%s\n", id);
			strcpy(tempNode->id, id);
			unique = TRUE;
		}
	}

	printf("Enter the item name: \n");
	if(getString(name, NAMELEN) == RTM)
	{
		free(tempNode);
		return RTM;
	}
	else
	{
		strcpy(tempNode->name, name);
	}

	printf("Enter the description: \n");
	if(getString(desc, DESCLEN) == RTM)
	{
		free(tempNode);
		return RTM;
	}
	else
	{
		strcpy(tempNode->desc, desc);
	}


	do
	{
		printf("Enter the price for this item [e.g 1.00]: \n");
		if(getString(inputPrice, 10) == RTM)
		{
			free(tempNode);
			return RTM;
		}
		dollar = strtok(inputPrice, ".");
		cent = strtok(NULL, ".");
	}while((valid = valid_price(system, dollar, cent)) == FALSE);

	if(valid != FALSE)
	{
		tempNode->price.dollars = strtol(dollar, &cptr, 10);
		tempNode->price.cents = strtol(cent, &cptr, 10);
		tempNode->on_hand = DEFAULT_STOCK_LEVEL;
		add_node(system, tempNode);
	}
	else
	{
		fprintf(stderr, "Error: ppd_options.c:273: Invalid Price");
		free(tempNode);
		return FALSE;
	}

	printf("This item %s - %s has now been added to the menu\n",
			tempNode->name, tempNode->desc);
    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true when removing an item succeeds and false when it does not
 **/
BOOLEAN remove_item(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    char input[IDLEN];
	struct ppd_node *curr_node, *found_node;

	curr_node = system->item_list->head;

	printf("Enter the item id of the item to remove from the menu:\n");

	/* Check if they cancelled the function */
	if(getString(input, IDLEN) == RTM)
	{
		return RTM;
	}

	/*
	 * Search for the node and return it if it finds it. Else it would return
	 * false meaning the item does not exist
	 */
	found_node = search_node(system, input);

	if(found_node == FALSE)
	{
		fprintf(stderr, "Error: ID not found. Please use option 1 to find the "
				"item and try again.\n");
		return FALSE;
	}

	/* If item exist attempt to remove the item */
	if(found_node->data->id == system->item_list->head->data->id)
	{
		system->item_list->head = curr_node->next;
	}
	else
	{
		while(curr_node != NULL)
		{
			if(curr_node->next == found_node)
			{
				curr_node->next = found_node->next;
				break;
			}
			curr_node = curr_node->next;
		}
	}
	system->item_list->count--;

	printf("\"%s - %s - %s\" has been removed\n", found_node->data->id,
			found_node->data->name, found_node->data->desc);
	free(found_node->data);
	free(found_node);

    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_stock(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    int i;
	struct ppd_node *curr_node;
	curr_node = system->item_list->head;
	for(i = 0; i < system->item_list->count; i++)
	{
		curr_node->data->on_hand = DEFAULT_STOCK_LEVEL;
		curr_node = curr_node->next;
	}

    return TRUE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail.
 **/
BOOLEAN reset_coins(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    int i;
	for(i = 0; i < NUM_DENOMS; i++)
	{
		system->cash_register[i].count = DEFAULT_COIN_COUNT;
	}
    return FALSE;
}

/**
 * @param system a pointer to a  ppd_system struct that contains 
 * all the information for managing the system.
 * @return true as this function cannot fail
 **/
BOOLEAN display_coins(struct ppd_system * system)
{
    /*
     * Please delete this default return value once this function has 
     * been implemented. Please note that it is convention that until
     * a function has been implemented it should return FALSE
     */
    int i;
	char *coins[] = {" 5 Cents", "10 Cents", "20 Cents", "50 Cents",
			"1 dollar", "2 dollar", "5 dollar", "10 dollar"};

	printf("Coins Summary\n");
	printf("-------------\n");
	printf("Denomination \t| Count\n");
	printf("-----------------------\n");
	for(i = 0; i < NUM_DENOMS; i++)
	{
		printf("%s\t|%d\n", coins[i], system->cash_register[i].count);

	}
	printf("\n");
    return FALSE;
}
