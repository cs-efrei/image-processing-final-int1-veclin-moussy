#include "bmp24.h"

void file_rawRead(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

void file_rawWrite(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}

t_pixel ** bmp24_allocateDataPixels(int width, int height) {
    t_pixel ** pixels = malloc(height * width * sizeof(t_pixel *));
    return pixels;
}

void bmp24_freeDataPixels (t_pixel ** pixels, int height) {
    free(pixels);
}

t_bmp24 * bmp24_allocate(int width, int height, int colorDepth) {
    t_pixel ** pixels = bmp24_allocateDataPixels(width, height);
    t_bmp24 * bmp24 = (t_bmp24 *) malloc(sizeof(t_bmp24));
    bmp24->width = width;
    bmp24->height = height;
    bmp24->colorDepth = colorDepth;
    bmp24->data = pixels;
    if (!(bmp24->data)) {
        free(bmp24);
        return NULL;
    }
    return bmp24;
}

void bmp24_free(t_bmp24 * bmp24) {
    free(bmp24->data);
    free(bmp24);
}

t_bmp24 *bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return NULL;
    }

    // Read headers
    t_bmp_header header;
    t_bmp_info header_info;

    file_rawRead(0x00, &header, sizeof(t_bmp_header), 1, file);
    file_rawRead(0x0E, &header_info, sizeof(t_bmp_info), 1, file);

    if (header.type != 0x4D42 || header_info.bits != 24) {
        fprintf(stderr, "Error: Not a valid 24-bit BMP file\n");
        fclose(file);
        return NULL;
    }

    // Allocate image structure
    t_bmp24 *img = bmp24_allocate(header_info.width, header_info.height, header_info.bits);
    if (!img) {
        fclose(file);
        return NULL;
    }

    img->header = header;
    img->header_info = header_info;

    // Read pixel data
    fseek(file, header.offset, SEEK_SET);
    for (int y = img->height - 1; y >= 0; --y) {
        printf("a");
        for (int x = 0; x < img->width; ++x) {
            uint8_t bgr[3];
            fread(bgr, sizeof(uint8_t), 3, file);
            img->data[y][x].blue = bgr[0];
            img->data[y][x].green = bgr[1];
            img->data[y][x].red = bgr[2];
        }
    }
    fclose(file);
    return img;
}

void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s for writing\n", filename);
        return;
    }

    // Write headers
    file_rawWrite(0x00, &img->header, sizeof(t_bmp_header), 1, file);
    file_rawWrite(0x0E, &img->header_info, sizeof(t_bmp_info), 1, file);

    // Write pixel data
    fseek(file, img->header.offset, SEEK_SET);
    for (int y = img->height - 1; y >= 0; --y) {
        for (int x = 0; x < img->width; ++x) {
            uint8_t bgr[3] = {
                img->data[y][x].blue,
                img->data[y][x].green,
                img->data[y][x].red
            };
            fwrite(bgr, sizeof(uint8_t), 3, file);
        }
    }

    fclose(file);
}
