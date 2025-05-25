/**
 * utils.h
 * Authors: Rafael Veclin, Clement Moussy
 *
 * Description:
 * Header file declaring utility functions used throughout the project.
 *
 * Role in the project:
 * Provides the interfaces for helper functions and menu controls
 * used in the main program.
 */

#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

/**
 * cap
 * Caps the sum of number1 and number2 to not exceed the given ceiling.
 *
 * Parameters:
 * number1 (int): First integer value.
 * number2 (int): Second integer value.
 * ceiling (int): Maximum allowed value.
 *
 * Returns:
 * int: The capped sum (sum if sum <= ceiling, else ceiling).
 */
int cap(int number1, int number2, int ceiling);

/**
 * clamp
 * Clamps the input value within a predefined range (implementation defined).
 *
 * Parameters:
 * value (int): The value to clamp.
 *
 * Returns:
 * int: The clamped value.
 */
int clamp(int value);

/**
 * create_kernel
 * Allocates and initializes a 3x3 kernel matrix with given data.
 *
 * Parameters:
 * data (float[3][3]): 2D array of floats to initialize the kernel.
 *
 * Returns:
 * float**: Pointer to dynamically allocated 3x3 kernel matrix.
 */
float** create_kernel(float data[3][3]);

/**
 * free_kernel
 * Frees the memory allocated for a kernel matrix.
 *
 * Parameters:
 * kernel (float**): The kernel matrix to free.
 */
void free_kernel(float** kernel);

/**
 * init_kernel
 * Initializes a 3x3 kernel with default values (implementation defined).
 *
 * Returns:
 * float**: Pointer to the initialized kernel matrix.
 */
float** init_kernel();

/**
 * main_menu
 * Displays and handles the main menu interface.
 */
void main_menu();

/**
 * menu_bmp8
 * Displays and handles the menu related to 8-bit BMP operations.
 */
void menu_bmp8();

/**
 * menu_bmp24
 * Displays and handles the menu related to 24-bit BMP operations.
 */
void menu_bmp24();

#endif // UTILS_H
