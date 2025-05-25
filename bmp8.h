/**
 * bmp8.h
 * Authors: Rafael Veclin, Clement Moussy
 *
 * Description:
 * Header file defining the structure and functions for handling 8-bit BMP images.
 * This includes loading, saving, freeing image data, displaying image information,
 * and performing basic image processing operations such as negative, brightness adjustment,
 * thresholding, and applying convolution filters.
 *
 * Role in the project:
 * Provides an interface for 8-bit BMP image manipulation and processing utilities.
 */

#ifndef BMP8_H
#define BMP8_H

#include "utils.h"

/**
 * t_bmp8
 * Structure representing an 8-bit BMP image.
 *
 * Members:
 * header (unsigned char[54]): BMP file header (54 bytes).
 * colorTable (unsigned char[1024]): Color palette for 8-bit BMP (256 colors * 4 bytes).
 * data (unsigned char*): Pointer to the raw pixel data.
 * width (unsigned int): Image width in pixels.
 * height (unsigned int): Image height in pixels.
 * colorDepth (unsigned int): Bits per pixel (should be 8).
 * dataSize (unsigned int): Size of the pixel data in bytes.
 */
typedef struct {
    unsigned char header[54];
    unsigned char colorTable[1024];
    unsigned char *data;
    unsigned int width;
    unsigned int height;
    unsigned int colorDepth;
    unsigned int dataSize;
} t_bmp8;

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
t_bmp8 * bmp8_loadImage(const char *filename);

/**
 * bmp8_saveImage
 * Saves an 8-bit BMP image to a file.
 *
 * Parameters:
 * filename (const char*): Destination file path.
 * img (t_bmp8*): Pointer to the image to save.
 */
void bmp8_saveImage(const char *filename, t_bmp8 *img);

/**
 * bmp8_free
 * Frees memory allocated for the BMP image data.
 *
 * Parameters:
 * img (t_bmp8*): Pointer to the image to free.
 */
void bmp8_free(t_bmp8 *img);

/**
 * bmp8_printInfo
 * Prints information about the BMP image to the console.
 *
 * Parameters:
 * img (t_bmp8*): Pointer to the image.
 */
void bmp8_printInfo(t_bmp8 *img);

/**
 * bmp8_negative
 * Applies a negative effect to the image by inverting pixel values.
 *
 * Parameters:
 * img (t_bmp8*): Pointer to the image to modify.
 */
void bmp8_negative(t_bmp8 *img);

/**
 * bmp8_brightness
 * Adjusts the brightness of the image by adding a value to each pixel.
 *
 * Parameters:
 * img (t_bmp8*): Pointer to the image to modify.
 * value (int): Amount to adjust brightness by (positive or negative).
 */
void bmp8_brightness(t_bmp8 *img, int value);

/**
 * bmp8_threshold
 * Applies a threshold filter to the image.
 *
 * Parameters:
 * img (t_bmp8*): Pointer to the image to modify.
 * threshold (int): Threshold value (0-255).
 */
void bmp8_threshold(t_bmp8 *img, int threshold);

/**
 * bmp8_applyFilter
 * Applies a convolution filter to the image using the provided kernel.
 *
 * Parameters:
 * img (t_bmp8*): Pointer to the image to modify.
 * kernel (float**): 3x3 convolution kernel.
 */
void bmp8_applyFilter(t_bmp8 *img, float **kernel);

#endif // BMP8_H
