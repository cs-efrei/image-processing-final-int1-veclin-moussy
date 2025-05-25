/**
* bmp24.c
 * Author: Clement Moussy
 *
 * Description:
 * Implementation of functions for manipulating 24-bit BMP images.
 * Provides memory management for pixel data, reading/writing BMP file headers and pixel data,
 * and common image processing operations such as negative, grayscale conversion,
 * brightness adjustment, and applying convolution filters.
 *
 * Role in the project:
 * Core image handling module for 24-bit BMP images, enabling reading from disk,
 * in-memory manipulation, and saving back to disk with various image processing utilities.
 */


#include "bmp24.h"


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


/**
 * bmp24_freeDataPixels
 * Frees memory allocated for a 2D array of pixels.
 *
 * Parameters:
 * pixels (t_pixel**): 2D pixel array to free.
 * height (int): Number of rows.
 */
void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    if (pixels) {
        for (int i = 0; i < height; i++) {
            free(pixels[i]);
        }
        free(pixels);
    }
}


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


/**
 * bmp24_free
 * Frees memory allocated for a 24-bit BMP image structure and its data.
 *
 * Parameters:
 * img (t_bmp24*): Pointer to the BMP image to free.
 */
void bmp24_free(t_bmp24 *img) {
    if (img) {
        bmp24_freeDataPixels(img->data, img->height);
        free(img);
    }
}


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
void file_rawRead(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file) {
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}


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
void file_rawWrite(uint32_t position, void *buffer, uint32_t size, size_t n, FILE *file) {
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}


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
void bmp24_readPixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    uint8_t bgr[3];
    fread(bgr, sizeof(uint8_t), 3, file);
    image->data[y][x].blue = bgr[0];
    image->data[y][x].green = bgr[1];
    image->data[y][x].red = bgr[2];
}


/**
 * bmp24_readPixelData
 * Reads pixel data from file into the BMP image.
 *
 * Parameters:
 * image (t_bmp24*): Image to fill pixel data.
 * file (FILE*): File pointer to read from.
 */
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
void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    uint8_t bgr[3];
    bgr[0] = image->data[y][x].blue;
    bgr[1] = image->data[y][x].green;
    bgr[2] = image->data[y][x].red;
    fwrite(bgr, sizeof(uint8_t), 3, file);
}


/**
 * bmp24_writePixelData
 * Writes pixel data from the BMP image to the file.
 *
 * Parameters:
 * image (t_bmp24*): Image providing pixel data.
 * file (FILE*): File pointer to write to.
 */
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
t_bmp24 *bmp24_loadImage(const char *filename) {
    // Load a BMP image from file
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


/**
 * bmp24_saveImage
 * Saves a 24-bit BMP image to a file.
 *
 * Parameters:
 * img (t_bmp24*): Image to save.
 * filename (const char*): Destination file path.
 */
void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    // Save a BMP image to file
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


/**
 * bmp24_negative
 * Applies negative effect to the image by inverting each pixel's colors.
 *
 * Parameters:
 * img (t_bmp24*): Image to modify.
 */
void bmp24_negative (t_bmp24* img) {
    //a function to inverse the colors in a 24 bit depth image
    for (int y = 0; y<img -> height; y++) {
        for (int x = 0; x<img -> width; x++) {
            (img -> data[y][x]).red = 255 - (img -> data[y][x]).red;
            (img -> data[y][x]).green = 255 - (img -> data[y][x]).green;
            (img -> data[y][x]).blue = 255 - (img -> data[y][x]).blue;
        }
    }
}


/**
 * bmp24_grayscale
 * Converts the image to grayscale.
 *
 * Parameters:
 * img (t_bmp24*): Image to modify.
 */
void bmp24_grayscale (t_bmp24* img) {
    //a function to make an image grayscale
    for (int y = 0; y<img -> height; y++) {
        for (int x = 0; x<img -> width; x++) {
            //calculate the average of all 3 primary colors
            int avgval = ((img -> data[y][x]).blue + (img -> data[y][x]).green + (img -> data[y][x]).red)/3;
            (img -> data[y][x]).red = avgval;
            (img -> data[y][x]).green = avgval;
            (img -> data[y][x]).blue = avgval;
        }
    }
}


/**
 * bmp24_brightness
 * Adjusts the brightness of the image by adding a value to each pixel color component.
 *
 * Parameters:
 * img (t_bmp24*): Image to modify.
 * value (int): Amount to adjust brightness by.
 */
void bmp24_brightness (t_bmp24 * img, int value) {
    //a function to add brightness to every pixel, uses the cap function to cap the max brightness
    for (int y = 0; y<img -> height; y++) {
        for (int x = 0; x<img -> width; x++) {
            (img -> data[y][x]).red = cap((img -> data[y][x]).red,value,255);
            (img -> data[y][x]).green = cap((img -> data[y][x]).green,value,255);
            (img -> data[y][x]).blue = cap((img -> data[y][x]).blue,value,255);
        }
    }
}


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
t_pixel bmp24_convolution(t_bmp24* img, int x, int y, float** kernel, int kernelSize) {
    float sum_red = 0.0f;
    float sum_green = 0.0f;
    float sum_blue = 0.0f;

    int radius = kernelSize / 2;

    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {
            // Calculate neighbor coordinates
            int nx = x + i;
            int ny = y + j;

            // Handle edge pixels by clamping coordinates
            if (nx < 0) nx = 0;
            if (ny < 0) ny = 0;
            if (nx >= img->width) nx = img->width - 1;
            if (ny >= img->height) ny = img->height - 1;

            // Get the kernel value for this position
            float kernel_val = kernel[i + radius][j + radius];

            // Get the pixel and multiply by kernel value
            t_pixel pixel = img->data[ny][nx];
            sum_red += pixel.red * kernel_val;
            sum_green += pixel.green * kernel_val;
            sum_blue += pixel.blue * kernel_val;
        }
    }

    // Create and return the resulting pixel
    t_pixel result;
    result.red = clamp(sum_red);
    result.green = clamp(sum_green);
    result.blue = clamp(sum_blue);

    return result;
}


/**
 * bmp24_apply_filter
 * Applies a convolution filter to the entire image.
 *
 * Parameters:
 * img (t_bmp24*): Image to modify.
 * kernelSize (int): Size of the convolution kernel.
 */
void bmp24_apply_filter(t_bmp24* img, int kernelSize) {
    if (kernelSize <= (img->height /2)) {
        float** kernel = init_kernel();
        // Create a temporary copy of the image to store results
        t_pixel** temp = (t_pixel**)malloc(img->height * sizeof(t_pixel*));
        for (int y = 0; y < img->height; y++) {
            temp[y] = (t_pixel*)malloc(img->width * sizeof(t_pixel));
            for (int x = 0; x < img->width; x++) {
                //apply convolution to each pixel
                temp[y][x] = bmp24_convolution(img, x, y, kernel, kernelSize);
            }
        }

        // Copy the results back to the original image
        for (int y = 0; y < img->height; y++) {
            for (int x = 0; x < img->width; x++) {
                img->data[y][x] = temp[y][x];
            }
            free(temp[y]);
        }
        free(temp);
    } else {
        fprintf(stderr, "Error: KernelSize bigger than the image, try again.\n");
    }
}


/**
 * bmp24_printInfo
 * Prints image information to the console.
 *
 * Parameters:
 * img (t_bmp24*): Image to display info about.
 */
void bmp24_printInfo(t_bmp24 *img) {
    if (!img) {
        printf("No image loaded.\n");
        return;
    }
    printf("BMP24 Image Info:\n");
    printf("Width: %d px\n", img->width);
    printf("Height: %d px\n", img->height);
    printf("Color Depth: %d bits\n", img->colorDepth);
    printf("File Size: %u bytes\n", img->header.size);
    printf("Image Size (raw): %u bytes\n", img->header_info.imagesize);
}