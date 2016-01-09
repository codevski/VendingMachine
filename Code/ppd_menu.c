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

#include "ppd_menu.h"

/**
 * @file ppd_menu.c handles the initialised and management of the menu 
 * array
 **/

/**
 * @param menu the menu item array to initialise
 **/
void init_menu( struct menu_item* menu)
{
	int i;
	char *name[] = {"Display Items", "Purchase Items", "Save and Exit",
			"Add Item", "Remove Item", "Display Coins", "Reset Stock",
			"Reset Coins", "Abort Program"};
	BOOLEAN (*function[])(struct ppd_system*) =
	{
			display_items, purchase_item, save_system, add_item, remove_item,
			display_coins, reset_stock, reset_coins,
	};

	for( i = 0; i < MENU_OPTS; i++)
	{
		strcpy(menu[i].name, name[i]);
		menu[i].function = function[i];
	}
}

void print_menu(struct menu_item* menu)
{
	/*
	 * Function to print out the function pointer menu
	 */
	int i;

	printf("\nMain Menu:\n");
	for(i = 0; i <= MENU_OPTS_NORM; i++)
	{
		printf("  %d. %s\n", i+1, menu[i].name);
	}

	printf("Administrator-Only Menu:\n");
	for(i = MENU_OPTS_ADMIN; i < MENU_OPTS; i ++)
	{
		printf("  %d. %s\n", i+1, menu[i].name);
	}
}

int get_menu_option(struct menu_item* menu)
{
	/*
	 * Function that receives the users input and calls the correct function
	 * to activate
	 */
	int choice, choice_result;
	BOOLEAN valid_result = FALSE;
	char *prompt = "Select your option (1-9)\n";
	while(!valid_result)
	{
		choice_result = getInteger(&choice, MENU_INPUT_LEN, prompt, 1, 9);
		if(choice_result == SUCCESS)
		{
			valid_result = TRUE;
		}
		else
		{
			printf("Invalid Input! Please try again\n");
			print_menu(menu);
		}
	}
	return choice;
}
