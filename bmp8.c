#include "bmp8.h"

t_bmp8 *bmp8_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    t_bmp8 *img = (t_bmp8 *)malloc(sizeof(t_bmp8));
    if (!img) {
        perror("Error allocating memory for image");
        fclose(file);
        return NULL;
    }

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