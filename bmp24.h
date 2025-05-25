/**
 * bmp24.h
 * Author: Clement Moussy
 *
 * Description:
 * Header file defining structures and functions for handling 24-bit BMP images.
 * This includes BMP file and info headers, pixel data structures, allocation,
 * reading/writing pixels and images, and basic image processing functions such as
 * negative, grayscale, brightness adjustment, and convolution filtering.
 *
 * Role in the project:
 * Provides the core interface and utilities for manipulating 24-bit BMP images.
 */

#ifndef BMP24_H
#define BMP24_H

#include "utils.h"

/**
 * t_bmp_header
 * Structure representing the BMP file header (14 bytes).
 */
typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} t_bmp_header;

/**
 * t_bmp_info
 * Structure representing the BMP info header.
 */
typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits;
    uint32_t compression;
    uint32_t imagesize;
    int32_t xresolution;
    int32_t yresolution;
    uint32_t ncolors;
    uint32_t importantcolors;
} t_bmp_info;

/**
 * t_pixel
 * Structure representing a pixel in 24-bit BMP format (RGB).
 */
typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} t_pixel;

/**
 * t_bmp24
 * Structure representing a 24-bit BMP image.
 *
 * Members:
 * header (t_bmp_header): BMP file header.
 * header_info (t_bmp_info): BMP info header.
 * width (int): Image width.
 * height (int): Image height.
 * colorDepth (int): Bits per pixel (should be 24).
 * data (t_pixel**): 2D array of pixels.
 */
typedef struct {
    t_bmp_header header;
    t_bmp_info header_info;
    int width;
    int height;
    int colorDepth;
    t_pixel **data;
} t_bmp24;

// Offsets for BMP header fields
#define BITMAP_MAGIC 0x00
#define BITMAP_SIZE 0x02
#define BITMAP_OFFSET 0x0A
#define BITMAP_WIDTH 0x12
#define BITMAP_HEIGHT 0x16
#define BITMAP_DEPTH 0x1C
#define BITMAP_SIZE_RAW 0x22

// BMP constants
#define BMP_TYPE 0x4D42
#define HEADER_SIZE 0x0E
#define INFO_SIZE 0x28
#define DEFAULT_DEPTH 0x18

/**
 * bmp24_allocateDataPixels
 * Allocates memory for a 2D array of pixels.
 *
 * Parameters:
 * width (int): Width of the image.
 * height (int): Height of the image.
 *
 * Returns:
 * t_pixel**: Pointer to allocated 2D pixel array.
 */
t_pixel ** bmp24_allocateDataPixels(int width, int height);

/**
 * bmp24_freeDataPixels
 * Frees memory allocated for a 2D array of pixels.
 *
 * Parameters:
 * pixels (t_pixel**): 2D pixel array to free.
 * height (int): Number of rows.
 */
void bmp24_freeDataPixels(t_pixel **pixels, int height);

/**
 * bmp24_allocate
 * Allocates memory for a 24-bit BMP image structure and its pixel data.
 *
 * Parameters:
 * width (int): Image width.
 * height (int): Image height.
 * colorDepth (int): Color depth (usually 24).
 *
 * Returns:
 * t_bmp24*: Pointer to allocated BMP image.
 */
t_bmp24 * bmp24_allocate(int width, int height, int colorDepth);

/**
 * bmp24_free
 * Frees memory allocated for a 24-bit BMP image structure and its data.
 *
 * Parameters:
 * img (t_bmp24*): Pointer to the BMP image to free.
 */
void bmp24_free(t_bmp24 *img);

/**
 * bmp24_readPixelValue
 * Reads a single pixel value from file at position (x, y) into image data.
 *
 * Parameters:
 * image (t_bmp24*): Image to store the pixel.
 * x (int): X-coordinate of the pixel.
 * y (int): Y-coordinate of the pixel.
 * file (FILE*): File pointer to read from.
 */
void bmp24_readPixelValue(t_bmp24 *image, int x, int y, FILE *file);

/**
 * bmp24_readPixelData
 * Reads pixel data from file into the BMP image.
 *
 * Parameters:
 * image (t_bmp24*): Image to fill pixel data.
 * file (FILE*): File pointer to read from.
 */
void bmp24_readPixelData(t_bmp24 *image, FILE *file);

/**
 * bmp24_writePixelValue
 * Writes a single pixel value to file from image data at position (x, y).
 *
 * Parameters:
 * image (t_bmp24*): Image providing the pixel.
 * x (int): X-coordinate of the pixel.
 * y (int): Y-coordinate of the pixel.
 * file (FILE*): File pointer to write to.
 */
void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *file);

/**
 * bmp24_writePixelData
 * Writes pixel data from the BMP image to the file.
 *
 * Parameters:
 * image (t_bmp24*): Image providing pixel data.
 * file (FILE*): File pointer to write to.
 */
void bmp24_writePixelData(t_bmp24 *image, FILE *file);

/**
 * bmp24_loadImage
 * Loads a 24-bit BMP image from a file.
 *
 * Parameters:
 * filename (const char*): Path to the BMP file.
 *
 * Returns:
 * t_bmp24*: Pointer to the loaded BMP image, or NULL on failure.
 */
t_bmp24 * bmp24_loadImage(const char *filename);

/**
 * bmp24_saveImage
 * Saves a 24-bit BMP image to a file.
 *
 * Parameters:
 * img (t_bmp24*): Image to save.
 * filename (const char*): Destination file path.
 */
void bmp24_saveImage(t_bmp24 *img, const char *filename);

/**
 * file_rawRead
 * Reads raw data from a specific position in a file.
 *
 * Parameters:
 * position (uint32_t): File offset to start reading from.
 * buffer (void*): Pointer to buffer where data is stored.
 * size (uint32_t): Size of each element to read.
 * n (size_t): Number of elements to read.
 * file (FILE*): File pointer.
 */
void file_rawRead(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file);

/**
 * file_rawWrite
 * Writes raw data to a specific position in a file.
 *
 * Parameters:
 * position (uint32_t): File offset to start writing at.
 * buffer (void*): Pointer to buffer containing data.
 * size (uint32_t): Size of each element to write.
 * n (size_t): Number of elements to write.
 * file (FILE*): File pointer.
 */
void file_rawWrite(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file);

/**
 * bmp24_negative
 * Applies negative effect to the image by inverting each pixel's colors.
 *
 * Parameters:
 * img (t_bmp24*): Image to modify.
 */
void bmp24_negative (t_bmp24* img);

/**
 * bmp24_grayscale
 * Converts the image to grayscale.
 *
 * Parameters:
 * img (t_bmp24*): Image to modify.
 */
void bmp24_grayscale (t_bmp24* img);

/**
 * bmp24_brightness
 * Adjusts the brightness of the image by adding a value to each pixel color component.
 *
 * Parameters:
 * img (t_bmp24*): Image to modify.
 * value (int): Amount to adjust brightness by.
 */
void bmp24_brightness (t_bmp24 * img, int value);

/**
 * bmp24_convolution
 * Applies a convolution kernel at pixel (x, y).
 *
 * Parameters:
 * img (t_bmp24*): Image to process.
 * x (int): X-coordinate of the pixel.
 * y (int): Y-coordinate of the pixel.
 * kernel (float**): Convolution kernel.
 * kernelSize (int): Size of the kernel (assumed square).
 *
 * Returns:
 * t_pixel: Resulting pixel after applying convolution.
 */
t_pixel bmp24_convolution(t_bmp24* img, int x, int y, float** kernel, int kernelSize);

/**
 * bmp24_apply_filter
 * Applies a convolution filter to the entire image.
 *
 * Parameters:
 * img (t_bmp24*): Image to modify.
 * kernelSize (int): Size of the convolution kernel.
 */
void bmp24_apply_filter(t_bmp24* img, int kernelSize);

/**
 * bmp24_printInfo
 * Prints image information to the console.
 *
 * Parameters:
 * img (t_bmp24*): Image to display info about.
 */
void bmp24_printInfo(t_bmp24 *img);

#endif // BMP24_H
