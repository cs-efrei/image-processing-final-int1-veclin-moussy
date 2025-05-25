/**
* equalize24.c
 * Author: Rafael Veclin
 *
 * Description:
 * Implementation of functions for converting between RGB and YUV color spaces
 * and performing histogram equalization on the luminance channel of 24-bit BMP images.
 * The equalization improves image contrast while preserving color fidelity by
 * applying adjustments only on the luminance (Y) channel.
 *
 * Role in the project:
 * Provides core image processing functionality for contrast enhancement in 24-bit BMP images.
 */


#include "equalize24.h"


/**
 * rgb_to_yuv
 * Converts an RGB color value to YUV color space.
 *
 * Parameters:
 * r (unsigned int): Red channel value (0-255).
 * g (unsigned int): Green channel value (0-255).
 * b (unsigned int): Blue channel value (0-255).
 * y (float*): Pointer to store the resulting luminance (Y).
 * u (float*): Pointer to store the resulting chrominance (U).
 * v (float*): Pointer to store the resulting chrominance (V).
 */
void rgb_to_yuv(unsigned int r, unsigned int g, unsigned int b, float *y, float *u, float *v) {
    *y = 0.299f * r + 0.587f * g + 0.114f * b;
    *u = -0.14713f * r - 0.28886f * g + 0.436f * b;
    *v = 0.615f * r - 0.51499f * g - 0.10001f * b;
}


/**
 * yuv_to_rgb
 * Converts a YUV color value back to RGB color space.
 *
 * Parameters:
 * y (float): Luminance value.
 * u (float): Chrominance U value.
 * v (float): Chrominance V value.
 * r (unsigned int*): Pointer to store the red channel value (0-255).
 * g (unsigned int*): Pointer to store the green channel value (0-255).
 * b (unsigned int*): Pointer to store the blue channel value (0-255).
 */
void yuv_to_rgb(float y, float u, float v, unsigned int *r, unsigned int *g, unsigned int *b) {
    int rr = (int)round(y + 1.13983f * v);
    int gg = (int)round(y - 0.39465f * u - 0.58060f * v);
    int bb = (int)round(y + 2.03211f * u);

    // Clamp to 0-255
    if (rr < 0) rr = 0;
    if (rr > 255) rr = 255;
    if (gg < 0) gg = 0;
    if (gg > 255) gg = 255;
    if (bb < 0) bb = 0;
    if (bb > 255) bb = 255;

    *r = (unsigned int)rr;
    *g = (unsigned int)gg;
    *b = (unsigned int)bb;
}


/**
 * bmp24_computeHistogram
 * Computes the histogram of the luminance (Y) channel from a 24-bit BMP image.
 *
 * Parameters:
 * img (t_bmp24*): Pointer to the BMP24 image.
 *
 * Returns:
 * unsigned int*: Pointer to an array of size 256 representing the luminance histogram.
 */
unsigned int * bmp24_computeHistogram(t_bmp24 *img) {
    unsigned int *histogram = (unsigned int*)calloc(256, sizeof(unsigned int));
    if (!histogram) return NULL;

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            t_pixel px = img->data[y][x];
            float Y, U, V;
            rgb_to_yuv(px.red, px.green, px.blue, &Y, &U, &V);

            int yValue = (int)round(Y);
            if (yValue < 0) yValue = 0;
            if (yValue > 255) yValue = 255;

            histogram[yValue]++;
        }
    }
    return histogram;
}


/**
 * bmp24_computeCDF
 * Computes the cumulative distribution function (CDF) from a histogram.
 *
 * Parameters:
 * hist (unsigned int*): Pointer to the histogram array (size 256).
 *
 * Returns:
 * unsigned int*: Pointer to an array of size 256 representing the CDF.
 */
unsigned int * bmp24_computeCDF(unsigned int *hist) {
    unsigned int *cdf = (unsigned int*)calloc(256, sizeof(unsigned int));
    unsigned int *hist_eq = (unsigned int*)calloc(256, sizeof(unsigned int));
    if (!cdf || !hist_eq) {
        free(cdf);
        free(hist_eq);
        return NULL;
    }

    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i-1] + hist[i];
    }

    unsigned int cdfmin = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] != 0) {
            cdfmin = cdf[i];
            break;
        }
    }

    unsigned int N = cdf[255];
    for (int i = 0; i < 256; i++) {
        hist_eq[i] = (unsigned int)round(((double)(cdf[i] - cdfmin) / (N - cdfmin)) * 255);
        if (hist_eq[i] > 255) hist_eq[i] = 255;
        if (hist_eq[i] < 0) hist_eq[i] = 0;
    }

    free(cdf);
    return hist_eq;
}


/**
 * bmp24_equalize
 * Performs histogram equalization on the luminance channel of a 24-bit BMP image in-place,
 * adjusting contrast while preserving color information.
 *
 * Parameters:
 * img (t_bmp24*): Pointer to the BMP24 image to equalize.
 */
void bmp24_equalize(t_bmp24 *img) {
    unsigned int *hist = bmp24_computeHistogram(img);
    if (!hist) return;

    unsigned int *hist_eq = bmp24_computeCDF(hist);
    if (!hist_eq) {
        free(hist);
        return;
    }

    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            t_pixel px = img->data[y][x];
            float Y, U, V;
            rgb_to_yuv(px.red, px.green, px.blue, &Y, &U, &V);

            // Equalize luminance channel
            int yValue = (int)round(Y);
            if (yValue < 0) yValue = 0;
            if (yValue > 255) yValue = 255;

            unsigned int y_eq = hist_eq[yValue];

            // Convert back to RGB using equalized luminance
            unsigned int r, g, b;
            yuv_to_rgb((float)y_eq, U, V, &r, &g, &b);

            img->data[y][x].red = (uint8_t)r;
            img->data[y][x].green = (uint8_t)g;
            img->data[y][x].blue = (uint8_t)b;
        }
    }

    free(hist_eq);
    free(hist);
}
