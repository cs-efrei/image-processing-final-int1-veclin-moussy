/**
* equalize8.h
 * Author: Rafael Veclin
 *
 * Description:
 * Header file declaring functions for histogram computation and histogram equalization
 * on 8-bit BMP images.
 *
 * Role in the project:
 * Provides an interface for performing histogram-based contrast equalization on 8-bit BMP images.
 */

#ifndef EQUALIZE8_H
#define EQUALIZE8_H

#include "bmp8.h"

/**
 * bmp8_computeHistogram
 * Computes the histogram of pixel intensities for an 8-bit BMP image.
 *
 * Parameters:
 * img (t_bmp8*): Pointer to the BMP image.
 *
 * Returns:
 * unsigned int*: Pointer to an array of size 256 representing the histogram (frequency of each intensity).
 */
unsigned int * bmp8_computeHistogram(t_bmp8 * img);

/**
 * bmp8_computeCDF
 * Computes the cumulative distribution function (CDF) from a histogram.
 *
 * Parameters:
 * hist (unsigned int*): Pointer to the histogram array (size 256).
 *
 * Returns:
 * unsigned int*: Pointer to an array of size 256 representing the CDF.
 */
unsigned int * bmp8_computeCDF(unsigned int * hist);

/**
 * bmp8_equalize
 * Performs histogram equalization on an 8-bit BMP image to enhance contrast.
 *
 * Parameters:
 * img (t_bmp8*): Pointer to the BMP image to equalize.
 */
void bmp8_equalize(t_bmp8 * img);

#endif // EQUALIZE8_H
