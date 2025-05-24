#include "bmp24.h"

// Allocate memory for pixel data
t_pixel **bmp24_allocateDataPixels(int width, int height) {
    t_pixel **pixels = malloc(height * sizeof(t_pixel *));
    if (!pixels) {
        fprintf(stderr, "Error: Unable to allocate memory for pixel rows.\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        pixels[i] = malloc(width * sizeof(t_pixel));
        if (!pixels[i]) {
            fprintf(stderr, "Error: Unable to allocate memory for pixel columns.\n");
            for (int j = 0; j < i; j++) {
                free(pixels[j]);
            }
            free(pixels);
            return NULL;
        }
    }
    return pixels;
}

// Free memory allocated for pixel data
void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    if (pixels) {
        for (int i = 0; i < height; i++) {
            free(pixels[i]);
        }
        free(pixels);
    }
}

// Allocate memory for a BMP image
t_bmp24 *bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *img = malloc(sizeof(t_bmp24));
    if (!img) {
        fprintf(stderr, "Error: Unable to allocate memory for BMP image.\n");
        return NULL;
    }

    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;
    img->data = bmp24_allocateDataPixels(width, height);
    if (!img->data) {
        free(img);
        return NULL;
    }

    return img;
}

// Free memory allocated for a BMP image
void bmp24_free(t_bmp24 *img) {
    if (img) {
        bmp24_freeDataPixels(img->data, img->height);
        free(img);
    }
}

// Read raw data from file at a specific position
void file_rawRead(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

// Write raw data to file at a specific position
void file_rawWrite(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}

// Read a single pixel value from file
void bmp24_readPixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    uint8_t bgr[3];
    fread(bgr, sizeof(uint8_t), 3, file);
    image->data[y][x].blue = bgr[0];
    image->data[y][x].green = bgr[1];
    image->data[y][x].red = bgr[2];
}

// Read all pixel data from file
void bmp24_readPixelData(t_bmp24 *image, FILE *file) {
    int width = image->width;
    int height = image->height;
    int padding = (4 - (width * 3) % 4) % 4;

    fseek(file, image->header.offset, SEEK_SET);
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            bmp24_readPixelValue(image, x, y, file);
        }
        fseek(file, padding, SEEK_CUR);
    }
}

// Write a single pixel value to file
void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    uint8_t bgr[3];
    bgr[0] = image->data[y][x].blue;
    bgr[1] = image->data[y][x].green;
    bgr[2] = image->data[y][x].red;
    fwrite(bgr, sizeof(uint8_t), 3, file);
}

// Write all pixel data to file
void bmp24_writePixelData(t_bmp24 *image, FILE *file) {
    int width = image->width;
    int height = image->height;
    int padding = (4 - (width * 3) % 4) % 4;
    uint8_t pad[3] = {0, 0, 0};

    fseek(file, image->header.offset, SEEK_SET);
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            bmp24_writePixelValue(image, x, y, file);
        }
        fwrite(pad, sizeof(uint8_t), padding, file);
    }
}

// Load a BMP image from file
t_bmp24 *bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s for reading.\n", filename);
        return NULL;
    }

    t_bmp_header header;
    t_bmp_info header_info;

    file_rawRead(BITMAP_MAGIC, &header.type, sizeof(uint16_t), 1, file);
    if (header.type != BMP_TYPE) {
        fprintf(stderr, "Error: File %s is not a valid BMP file.\n", filename);
        fclose(file);
        return NULL;
    }

    file_rawRead(BITMAP_SIZE, &header.size, sizeof(uint32_t), 1, file);
    file_rawRead(BITMAP_OFFSET, &header.offset, sizeof(uint32_t), 1, file);
    file_rawRead(HEADER_SIZE, &header_info, sizeof(t_bmp_info), 1, file);

    int width = header_info.width;
    int height = header_info.height;
    int colorDepth = header_info.bits;

    if (colorDepth != 24) {
        fprintf(stderr, "Error: Only 24-bit BMP files are supported.\n");
        fclose(file);
        return NULL;
    }

    t_bmp24 *image = bmp24_allocate(width, height, colorDepth);
    if (!image) {
        fclose(file);
        return NULL;
    }

    image->header = header;
    image->header_info = header_info;

    bmp24_readPixelData(image, file);
    fclose(file);
    return image;
}

// Save a BMP image to file
void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s for writing.\n", filename);
        return;
    }

    file_rawWrite(BITMAP_MAGIC, &img->header.type, sizeof(uint16_t), 1, file);
    file_rawWrite(BITMAP_SIZE, &img->header.size, sizeof(uint32_t), 1, file);
    file_rawWrite(BITMAP_OFFSET, &img->header.offset, sizeof(uint32_t), 1, file);
    file_rawWrite(HEADER_SIZE, &img->header_info, sizeof(t_bmp_info), 1, file);

    bmp24_writePixelData(img, file);
    fclose(file);
}
