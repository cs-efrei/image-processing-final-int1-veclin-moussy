/**
 * equalize24.h
 * Author: Rafael Veclin
 *
 * Description:
 * Header file declaring functions for color space conversion (RGB <-> YUV) and
 * histogram equalization on 24-bit BMP images. The equalization is applied to the luminance (Y) channel
 * to improve image contrast without distorting color information.
 *
 * Role in the project:
 * Provides interfaces to perform histogram equalization on 24-bit BMP images by
 * manipulating luminance while preserving color fidelity.
 */

#ifndef EQUALIZE24_H
#define EQUALIZE24_H

#include "bmp24.h"

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
void rgb_to_yuv(unsigned int r, unsigned int g, unsigned int b, float *y, float *u, float *v);

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
void yuv_to_rgb(float y, float u, float v, unsigned int *r, unsigned int *g, unsigned int *b);

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
unsigned int * bmp24_computeHistogram(t_bmp24 *img);

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
unsigned int * bmp24_computeCDF(unsigned int *hist);

/**
 * bmp24_equalize
 * Performs histogram equalization on the luminance channel of a 24-bit BMP image in-place,
 * adjusting contrast while preserving color information.
 *
 * Parameters:
 * img (t_bmp24*): Pointer to the BMP24 image to equalize.
 */
void bmp24_equalize(t_bmp24 *img);

#endif // EQUALIZE24_H
