/**
* main.c
 * Author: Rafael Veclin
 *
 * Description:
 * This is the entry point of the project. It initializes the application by
 * calling the main menu function from the utils module. The main function
 * serves to start the program's user interface and handle the initial user
 * interactions.
 *
 * Role in the project:
 * Acts as the launcher for the application, directing the flow to the main menu.
 */

#include "utils.h"

/**
 * main
 *
 * Starts the application by invoking the main menu.
 *
 * Returns:
 * 0 - Indicates successful execution of the program.
 */
int main() {
    main_menu();  // Call to display and handle the main menu interface
    return 0;
}
