#include "bmp24.h"
#include "bmp8.h"
#include "utils.h"

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