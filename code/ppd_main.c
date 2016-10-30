#include "ppd_main.h"
#include "ppd_menu.h"
#include "ppd_options.h"
#include "ppd_utility.h"

/**
 * @file ppd_main.c contains the main function implementation and any 
 * helper functions for main such as a display_usage() function.
 **/

/**
 * manages the running of the program, initialises data structures, loads
 * data and handles the processing of options. The bulk of this function
 * should simply be calling other functions to get the job done.
 **/
int main(int argc, char **argv)
{
    /* represents the data structures to manage the system */
    struct ppd_system system;
    struct menu_item menu[MENU_OPTS];
    int option;
    BOOLEAN quit;


	/* validate command line arguments */
    if (argc != 3)
    {
        printf("Incorrect Syntax: ./ppd <itemsfile> <coinsfile>\n");
        exit(0);
    }

    /* Assign file names into the list */
    system.stock_file_name = argv[1];
    system.coin_file_name = argv[2];

    /* init the system */
    system_init(&system);

    /* load data */
    if(load_data(&system, system.coin_file_name, system.stock_file_name) == FALSE)
    {
    	system_free(&system);
    	printf("Error Occured");
    	exit(0);
    }

    /* test if everything has been initialised correctly */

    /* initialise the menu system */
    init_menu(menu);

    /* loop, asking for options from the menu */
    /* run each option selected */
    /* until the user quits */
    quit = FALSE;

    do
    {
        print_menu(menu);
    	option = get_menu_option(menu);

    	if(option == 9)
    	{
    		quit = TRUE;
    		system_free(&system);
    	}
    	else if(option == 3)
		{
    		menu[option-1].function(&system);
    		quit = TRUE;
		}
    	else
    	{
    		menu[option-1].function(&system);
    	}
    }while (!quit);

    /* make sure you always free all memory and close all files 
     * before you exit the program
     */

    return EXIT_SUCCESS;
}
