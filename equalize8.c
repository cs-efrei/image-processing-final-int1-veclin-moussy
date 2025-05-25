/**
* equalize8.c
 * Author: Rafael Veclin
 *
 * Description:
 * Implements functions for computing histograms, cumulative distribution functions (CDF),
 * and performing histogram equalization on 8-bit BMP images to improve image contrast.
 *
 * Role in the project:
 * Provides the core logic to analyze pixel intensity distribution and enhance image quality
 * by spreading out intensity values.
 */


#include "equalize8.h"


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
unsigned int * bmp8_computeHistogram(t_bmp8 * img) {
    unsigned int * histogram = (unsigned int*)calloc(256, sizeof(unsigned int));
    int width = img->width;
    int height = img->height;

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            unsigned char pixelValue = img->data[y * width + x];
            histogram[pixelValue]++;
        }
    }
    return histogram;
}


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
unsigned int * bmp8_computeCDF(unsigned int * hist) {
    unsigned int * cdf = (unsigned int*)calloc(256, sizeof(unsigned int));
    unsigned int * hist_eq = (unsigned int*)calloc(256, sizeof(unsigned int));

    cdf[0] = hist[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i-1] + hist[i];
    }

    unsigned int cdfmin = 0;
    for (int i = 0; i < 256; i++) {
        if (cdf[i] != 0) {
            cdfmin = cdf[i];
            break; // cdf is sorted in increasing order, so the first non-0 value is the smallest one
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
 * bmp8_equalize
 * Performs histogram equalization on an 8-bit BMP image to enhance contrast.
 *
 * Parameters:
 * img (t_bmp8*): Pointer to the BMP image to equalize.
 */
void bmp8_equalize(t_bmp8 * img) {
    unsigned int * hist = bmp8_computeHistogram(img);
    unsigned int * hist_eq = bmp8_computeCDF(hist);
    for (int i = 0; i < img->dataSize; i++) {
        img->data[i] = hist_eq[img->data[i]] ;
    }
    free(hist_eq);
    free(hist);
}