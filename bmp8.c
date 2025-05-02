#include "bmp8.h"

t_bmp8 *bmp8_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file");
        return NULL;
    }

    t_bmp8 *img = (t_bmp8 *)malloc(sizeof(t_bmp8));

    // Read the header
    fread(img->header, sizeof(unsigned char), 54, file);

    // Read the color table
    fread(img->colorTable, sizeof(unsigned char), 1024, file);

    // Extract width, height, color depth, and data size from header
    img->width = *(unsigned int *)&img->header[18];
    img->height = *(unsigned int *)&img->header[22];
    img->colorDepth = *(unsigned int *)&img->header[28];
    img->dataSize = *(unsigned int *)&img->header[34];

    if (img->colorDepth != 8) {
        printf("Error: The image is not 8-bit grayscale.\n");
        free(img);
        fclose(file);
        return NULL;
    }

    // Allocate memory for pixel data
    img->data = (unsigned char *)malloc(img->dataSize);

    // Read the pixel data
    fread(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file);
    return img;
}

void bmp8_saveImage(const char *filename, t_bmp8 *img) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error opening file for writing");
        return;
    }

    fwrite(img->header, sizeof(unsigned char), 54, file);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file);
}

void bmp8_free(t_bmp8 *img) {
    if (img) {
        free(img->data);
        free(img);
    }
}

void bmp8_printInfo(t_bmp8 *img) {
    if (img) {
        printf("Image Info:\n");
        printf("Width: %u\n", img->width);
        printf("Height: %u\n", img->height);
        printf("Color Depth: %u\n", img->colorDepth);
        printf("Data Size: %u bytes\n", img->dataSize);
    }
}

void bmp8_negative(t_bmp8 *img) {
    if (!img || !img->data) return;

    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
    bmp8_saveImage("amg.bmp", img);
}

void bmp8_brightness(t_bmp8 *img, int value) {
    if (!img || !img->data) return;

    for (unsigned int i = 0; i < img->dataSize; ++i) {
        int newVal = img->data[i] + value;

        if (newVal > 255) newVal = 255;
        else if (newVal < 0) newVal = 0;

        img->data[i] = (unsigned char)newVal;
    }
    bmp8_saveImage("amg.bmp", img);
}

void bmp8_threshold(t_bmp8 *img, int threshold) {
    if (!img || !img->data) return;

    for (unsigned int i = 0; i < img->dataSize; ++i) {
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
    }
    bmp8_saveImage("amg.bmp", img);
}

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


void free_kernel(float** kernel) {
    for (int i = 0; i < 3; i++) {
        free(kernel[i]);
    }
    free(kernel);
}


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


void bmp8_applyFilter(t_bmp8 *img, float **kernel) {
    int n = 1;
    unsigned char *tempData = (unsigned char *)malloc(img->dataSize);
    int width = img->width;
    int height = img->height;

    for (int y = n; y < height - n; y++) {
        for (int x = n; x < width - n; x++) {
            float sum = 0.0;

            for (int ky = -n; ky <= n; ky++) {
                for (int kx = -n; kx <= n; kx++) {
                    unsigned char pixel = img->data[(y + ky) * width + (x + kx)];
                    sum += pixel * kernel[ky + n][kx + n];
                }
            }

            if (sum < 0) sum = 0;
            if (sum > 255) sum = 255;
            tempData[y * width + x] = (unsigned char)sum;
        }
    }

    for (int y = n; y < height - n; y++) {
        for (int x = n; x < width - n; x++) {
            img->data[y * width + x] = tempData[y * width + x];
        }
    }

    free(tempData);
    free_kernel(kernel);
}