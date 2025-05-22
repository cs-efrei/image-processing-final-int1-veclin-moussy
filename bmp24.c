#include "bmp24.h"

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
