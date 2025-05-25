#include "bmp8.h"
#include "utils.h"

int main() {
    t_bmp8* img = NULL;
    char filename[256];
    int choice;

    while (1) {
        print_menu();
        scanf("%d", &choice);
        getchar(); // Consume newline

        switch (choice) {
            case 1: // Load image
                printf("Enter image filename: ");
                fgets(filename, sizeof(filename), stdin);
                {
                    size_t len = strlen(filename);
                    if (len > 0 && filename[len-1] == '\n') {
                        filename[len-1] = '\0';
                    }
                }

                if (img) bmp8_free(img);
                img = bmp8_loadImage(filename);

                if (img) {
                    printf("Image loaded successfully!\n");
                    bmp8_printInfo(img);
                } else {
                    printf("Failed to load image.\n");
                }
                break;

            case 2: // Save image
                if (!img) {
                    printf("No image loaded!\n");
                    break;
                }

                printf("Enter output filename: ");
                fgets(filename, sizeof(filename), stdin);
                {
                    size_t len = strlen(filename);
                    if (len > 0 && filename[len-1] == '\n') {
                        filename[len-1] = '\0';
                    }
                }

                bmp8_saveImage(filename, img);
                printf("Image saved successfully!\n");
                break;

            case 3: // Apply filter
                if (!img) {
                    printf("No image loaded!\n");
                    break;
                }
                float** kernel = init_kernel();
                bmp8_applyFilter(img, kernel);
                printf("Filter applied successfully!\n");
                break;

            case 4: // Show image info
                if (img) {
                    bmp8_printInfo(img);
                } else {
                    printf("No image loaded!\n");
                }
                break;

            case 5:
                if (img) {
                    bmp8_equalize(img);
                } else {
                    printf("No image loaded!\n");
                }
                break;

            case 6: // Exit
                if (img) bmp8_free(img);
                printf("Exiting program.\n");
                return 0;

            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}