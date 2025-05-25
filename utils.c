#include "utils.h"

void print_menu() {
    printf("\nImage Processing Menu:\n");
    printf("1. Load image\n");
    printf("2. Save image\n");
    printf("3. Apply filter\n");
    printf("4. Show image info\n");
    printf("5. Exit\n");
    printf("Enter your choice: ");
}

int cap(int number1, int number2, int ceiling) {
    //a function to cap an addition of 2 numbers
    if ((number1 + number2) > ceiling) {
        return ceiling;
    }
    else {
        return number1 + number2;
    }
}

int clamp(int value) {
    // a function to clamp 8 bit values
    if (value < 0) return  0;
    if (value > 255) return  255;
    return value;
}