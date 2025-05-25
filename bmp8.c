/**
* bmp8.c
 * Authors: Rafael Veclin, Clement Moussy
 *
 * Description:
 * Implements functions for loading, saving, freeing, and processing 8-bit BMP images.
 * Includes image transformations such as negative, brightness adjustment, thresholding,
 * and applying convolution filters.
 *
 * Role in the project:
 * Provides the core functionality to manipulate 8-bit BMP images for the application.
 */


#include "bmp8.h"


/**
 * bmp8_loadImage
 * Loads an 8-bit BMP image from a file.
 *
 * Parameters:
 * filename (const char*): Path to the BMP file.
 *
 * Returns:
 * t_bmp8*: Pointer to the loaded image structure, or NULL on failure.
 */
t_bmp8 *bmp8_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error opening file.\n");
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


/**
 * bmp8_saveImage
 * Saves an 8-bit BMP image to a file.
 *
 * Parameters:
 * filename (const char*): Destination file path.
 * img (t_bmp8*): Pointer to the image to save.
 */
void bmp8_saveImage(const char *filename, t_bmp8 *img) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }

    fwrite(img->header, sizeof(unsigned char), 54, file);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);

    fclose(file);
}


/**
 * bmp8_free
 * Frees memory allocated for the BMP image data.
 *
 * Parameters:
 * img (t_bmp8*): Pointer to the image to free.
 */
void bmp8_free(t_bmp8 *img) {
    if (img) {
        free(img->data);
        free(img);
    }
}


/**
 * bmp8_printInfo
 * Prints information about the BMP image to the console.
 *
 * Parameters:
 * img (t_bmp8*): Pointer to the image.
 */
void bmp8_printInfo(t_bmp8 *img) {
    if (img) {
        printf("Image Info:\n");
        printf("Width: %u\n", img->width);
        printf("Height: %u\n", img->height);
        printf("Color Depth: %u\n", img->colorDepth);
        printf("Data Size: %u bytes\n", img->dataSize);
    }
}


/**
 * bmp8_negative
 * Applies a negative effect to the image by inverting pixel values.
 *
 * Parameters:
 * img (t_bmp8*): Pointer to the image to modify.
 */
void bmp8_negative(t_bmp8 *img) {
    if (!img || !img->data) return;

    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
}


/**
 * bmp8_brightness
 * Adjusts the brightness of the image by adding a value to each pixel.
 *
 * Parameters:
 * img (t_bmp8*): Pointer to the image to modify.
 * value (int): Amount to adjust brightness by (positive or negative).
 */
void bmp8_brightness(t_bmp8 *img, int value) {
    if (!img || !img->data) return;

    for (unsigned int i = 0; i < img->dataSize; ++i) {
        int newVal = img->data[i] + value;

        if (newVal > 255) newVal = 255;
        else if (newVal < 0) newVal = 0;

        img->data[i] = (unsigned char)newVal;
    }
}


/**
 * bmp8_threshold
 * Applies a threshold filter to the image.
 *
 * Parameters:
 * img (t_bmp8*): Pointer to the image to modify.
 * threshold (int): Threshold value (0-255).
 */
void bmp8_threshold(t_bmp8 *img, int threshold) {
    if (!img || !img->data) return;

    for (unsigned int i = 0; i < img->dataSize; ++i) {
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
    }
}


/**
 * bmp8_applyFilter
 * Applies a convolution filter to the image using the provided kernel.
 *
 * Parameters:
 * img (t_bmp8*): Pointer to the image to modify.
 * kernel (float**): 3x3 convolution kernel.
 */
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