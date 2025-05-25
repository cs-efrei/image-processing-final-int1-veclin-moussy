/**
* utils.c
 * Authors: Rafael Veclin, Clement Moussy
 *
 * Description:
 * Implements utility functions and menus for image processing.
 *
 * Role in the project:
 * Provides core helper functions and user interface handling for
 * loading, saving, processing, and displaying image information.
 */

#include "utils.h"
#include "equalize8.h"
#include "equalize24.h"

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
int cap(int number1, int number2, int ceiling) {
    //a function to cap an addition of 2 numbers
    if ((number1 + number2) > ceiling) {
        return ceiling;
    }
    else {
        return number1 + number2;
    }
}


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
int clamp(int value) {
    // a function to clamp 8 bit values
    if (value < 0) return  0;
    if (value > 255) return  255;
    return value;
}


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
float** create_kernel(float data[3][3]) {
    float** kernel = (float**)malloc(3 * sizeof(float*));
    for (int i = 0; i < 3; i++) {
        kernel[i] = (float*)malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = data[i][j];
        }
    }
    return kernel;
}


/**
 * free_kernel
 * Frees the memory allocated for a kernel matrix.
 *
 * Parameters:
 * kernel (float**): The kernel matrix to free.
 */
void free_kernel(float** kernel) {
    for (int i = 0; i < 3; i++) {
        free(kernel[i]);
    }
    free(kernel);
}


/**
 * init_kernel
 * Initializes a 3x3 kernel with default values (implementation defined).
 *
 * Returns:
 * float**: Pointer to the initialized kernel matrix.
 */
float** init_kernel() {
    float box_blur[3][3] = {
        {1.0/9, 1.0/9, 1.0/9},
        {1.0/9, 1.0/9, 1.0/9},
        {1.0/9, 1.0/9, 1.0/9}
    };

    float gaussian_blur[3][3] = {
        {1.0/16, 2.0/16, 1.0/16},
        {2.0/16, 4.0/16, 2.0/16},
        {1.0/16, 2.0/16, 1.0/16}
    };

    float outline[3][3] = {
        {-1, -1, -1},
        {-1,  8, -1},
        {-1, -1, -1}
    };

    float emboss[3][3] = {
        {-2, -1,  0},
        {-1,  1,  1},
        { 0,  1,  2}
    };

    float sharpen[3][3] = {
        { 0, -1,  0},
        {-1,  5, -1},
        { 0, -1,  0}
    };

    int choice;
    float** output = NULL;

    while (1) {
        printf("\nSelect a filter:\n");
        printf("1. Box blur\n");
        printf("2. Gaussian blur\n");
        printf("3. Outline\n");
        printf("4. Emboss\n");
        printf("5. Sharpen\n");
        printf("Enter your choice (1-5): ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch(choice) {
            case 1:
                output = create_kernel(box_blur);
            break;
            case 2:
                output = create_kernel(gaussian_blur);
            break;
            case 3:
                output = create_kernel(outline);
            break;
            case 4:
                output = create_kernel(emboss);
            break;
            case 5:
                output = create_kernel(sharpen);
            break;
            default:
                printf("Invalid choice. Please select a number between 1-5.\n");
            continue;
        }
        break;
    }

    return output;
}


/**
 * main_menu
 * Displays and handles the main menu interface.
 */
void main_menu() {
    while (1) {
        int formatChoice = 0;
        printf("Select image format to work on:\n");
        printf("1. BMP8 (8-bit grayscale)\n");
        printf("2. BMP24 (24-bit color)\n");
        printf("3. Exit\n");
        printf("Enter choice: ");

        // Si scanf échoue (retourne 0), on vide le buffer
        if (scanf("%d", &formatChoice) != 1) {
            printf("Invalid input! Please enter a number.\n\n");
            // Vider le buffer d'entrée
            while (getchar() != '\n');
            continue;
        }
        getchar(); // Consommer le '\n' restant

        switch (formatChoice) {
            case 1: menu_bmp8(); break;
            case 2: menu_bmp24(); break;
            case 3: printf("Exiting program.\n"); return;
            default: printf("Invalid choice! Please try again.\n");
        }
    }
}


/**
 * menu_bmp8
 * Displays and handles the menu related to 8-bit BMP operations.
 */
void menu_bmp8() {
    t_bmp8* img = NULL;
    char filename[256];
    int choice;

    while (1) {
        printf("\n-- BMP8 Menu --\n");
        printf("1. Load image\n");
        printf("2. Save image\n");
        printf("3. Apply image processing\n");
        printf("4. Show image info\n");
        printf("5. Return to main menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        getchar(); // consume newline

        switch (choice) {
            case 1:
                printf("Enter image filename: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;

                if (img) bmp8_free(img);
                img = bmp8_loadImage(filename);
                if (img) {
                    printf("Image loaded successfully!\n");
                    bmp8_printInfo(img);
                } else {
                    printf("Failed to load image.\n");
                }
                break;

            case 2:
                if (!img) {
                    printf("No image loaded!\n");
                    break;
                }
                printf("Enter output filename: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;

                bmp8_saveImage(filename, img);
                printf("Image saved successfully!\n");
                break;

            case 3:
                if (!img) {
                    printf("No image loaded!\n");
                    break;
                }
                {
                    int procChoice;
                    printf("\n-- Image Processing --\n");
                    printf("1. Apply convolution filter\n");
                    printf("2. Adjust brightness\n");
                    printf("3. Apply threshold\n");
                    printf("4. Convert to negative\n");
                    printf("5. Equalize histogram\n");
                    printf("Enter processing choice: ");

                    if (scanf("%d", &procChoice) != 1) {
                        printf("Invalid input! Please enter a number.\n");
                        while (getchar() != '\n');
                        break;
                    }
                    getchar(); // consume newline

                    switch (procChoice) {
                        case 1: {
                            float **kernel = init_kernel();
                            bmp8_applyFilter(img, kernel);
                            printf("Filter applied successfully!\n");
                            break;
                        }
                        case 2: {
                            int brightness;
                            printf("Enter brightness adjustment (-255 to 255): ");
                            if (scanf("%d", &brightness) != 1) {
                                printf("Invalid input!\n");
                                while (getchar() != '\n');
                                break;
                            }
                            getchar();
                            bmp8_brightness(img, brightness);
                            printf("Brightness adjusted successfully!\n");
                            break;
                        }
                        case 3: {
                            int threshold;
                            printf("Enter threshold value (0 to 255): ");
                            if (scanf("%d", &threshold) != 1) {
                                printf("Invalid input!\n");
                                while (getchar() != '\n');
                                break;
                            }
                            getchar();
                            bmp8_threshold(img, threshold);
                            printf("Threshold applied successfully!\n");
                            break;
                        }
                        case 4:
                            bmp8_negative(img);
                            printf("Negative conversion applied successfully!\n");
                            break;
                        case 5:
                            bmp8_equalize(img);
                            printf("Histogram equalization applied.\n");
                            break;
                        default:
                            printf("Invalid processing choice!\n");
                    }
                }
                break;

            case 4:
                if (img) {
                    bmp8_printInfo(img);
                } else {
                    printf("No image loaded!\n");
                }
                break;

            case 5:
                if (img) {
                    bmp8_free(img);
                    img = NULL;
                }
                return;

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}


/**
 * menu_bmp24
 * Displays and handles the menu related to 24-bit BMP operations.
 */
void menu_bmp24() {
    t_bmp24* img = NULL;
    char filename[256];
    int choice;

    while (1) {
        printf("\n-- BMP24 Menu --\n");
        printf("1. Load image\n");
        printf("2. Save image\n");
        printf("3. Apply image processing\n");
        printf("4. Show image info\n");
        printf("5. Return to main menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }
        getchar(); // consume newline

        switch (choice) {
            case 1:
                printf("Enter image filename: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;

                if (img) bmp24_free(img);
                img = bmp24_loadImage(filename);
                if (img) {
                    printf("Image loaded successfully!\n");
                    bmp24_printInfo(img);
                } else {
                    printf("Failed to load image.\n");
                }
                break;

            case 2:
                if (!img) {
                    printf("No image loaded!\n");
                    break;
                }
                printf("Enter output filename: ");
                fgets(filename, sizeof(filename), stdin);
                filename[strcspn(filename, "\n")] = 0;

                bmp24_saveImage(img, filename);
                printf("Image saved successfully!\n");
                break;

            case 3:
                if (!img) {
                    printf("No image loaded!\n");
                    break;
                }
                {
                    int processingChoice;
                    printf("\n-- Image Processing --\n");
                    printf("1. Apply convolution filter\n");
                    printf("2. Adjust brightness\n");
                    printf("3. Convert to negative\n");
                    printf("4. Convert to grayscale\n");
                    printf("5. Equalize histogram\n");
                    printf("Enter processing choice: ");

                    if (scanf("%d", &processingChoice) != 1) {
                        printf("Invalid input! Please enter a number.\n");
                        while (getchar() != '\n');
                        break;
                    }
                    getchar(); // consume newline

                    switch (processingChoice) {
                        case 1:
                            bmp24_apply_filter(img, 3);
                            printf("Filter applied successfully!\n");
                            break;
                        case 2: {
                            int brightness;
                            printf("Enter brightness adjustment (-255 to 255): ");
                            if (scanf("%d", &brightness) != 1) {
                                printf("Invalid input!\n");
                                while (getchar() != '\n');
                                break;
                            }
                            getchar();
                            bmp24_brightness(img, brightness);
                            printf("Brightness adjusted successfully!\n");
                            break;
                        }
                        case 3:
                            bmp24_negative(img);
                            printf("Negative conversion applied successfully!\n");
                            break;
                        case 4:
                            bmp24_grayscale(img);
                            printf("Grayscale conversion applied successfully!\n");
                            break;
                        case 5:
                            bmp24_equalize(img);
                            printf("Histogram equalization applied successfully!\n");
                            break;
                        default:
                            printf("Invalid processing choice!\n");
                    }
                }
                break;

            case 4:
                if (img) {
                    bmp24_printInfo(img);
                } else {
                    printf("No image loaded!\n");
                }
                break;

            case 5:
                if (img) {
                    bmp24_free(img);
                    img = NULL;
                }
                return;

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}
