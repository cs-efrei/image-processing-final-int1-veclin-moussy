# Image Processing in C

## General presentation

**Project title**: Image Processing in C

**Contributors**:
- **Rafael Véclin** (Meta122) : part 1, part 3, documentation
- **Clément Moussy** (Otherbug) : part 1, part 2

## Description

This project aims at providing tools to manipulate BMP images. It includes basic image operations such as negative, brightness adjustment, thresholding, filtering via convolution kernels, and histogram equalization on both grayscale and color images.

## Key features

- Support for 8-bit grayscale and 24-bit color BMP images
- Image processing functions: negative, brightness, threshold, grayscale, filtering with customizable kernels
- Histogram equalization on 8-bit and 24-bit images using RGB-to-YUV color space conversion
- Modular codebase with clear separation between utilities and image format handling

## Technologies used

- **Programming language**: C
- **Libraries**: Standard C libraries (`stdio.h`, `stdlib.h`, `string.h`, `stdint.h`, `math.h`)
- **Tools**:
    - GCC for compilation
    - Makefiles for build automation
    - Git for version control
    - CLion for development
    - A bit of GPT-4o for documentation and readability

## Installation

### Clone the repository

```bash
git clone https://github.com/Meta122/image-processing-veclin-moussy-int1.git
```

## How to use
Open the project, preferably in CLion, and run it. Choose if you want to work on an 8-bit or 24-bit image, and load that image (be careful to use ../ before the name if the image is at the beginning of the structure and .bmp at the end of the name). Then, process the image however you want, and save it (don't forget the .bmp extension !) before exiting the program.


## Technical documentation

### Game algorithm

The project is structured around a clear sequence of steps for image processing:

1. **Display main menu** for user interaction with options.
2. **Process user input** for selecting the desired operation.
3. **Perform image operation** based on user choice.
4. **Update image data in memory** with processed results.
5. **Save processed image** back to disk upon user request.
6. **Free allocated memory** before exiting.

### Functions

Below is a non-exhaustive list of key functions and their roles, given as example :

| Function prototype                                                          | Description                                                  |
|-----------------------------------------------------------------------------|--------------------------------------------------------------|
| `int clamp(int value);`                                                      | Clamps the integer value between 0 and 255.                  |
| `float** create_kernel(float data[3][3]);`                                  | Allocates and initializes a 3x3 convolution kernel from input data. |
| `void main_menu();`                                                          | Displays the main interactive menu and handles user input.   |
| `t_bmp8 * bmp8_loadImage(const char *filename);`                            | Loads an 8-bit BMP image from a file.                        |
| `void bmp8_negative(t_bmp8 *img);`                                          | Applies negative filter to an 8-bit image.                    |
| `unsigned int * bmp8_computeHistogram(t_bmp8 * img);`                       | Computes histogram of pixel intensities in an 8-bit image.   |
| `void rgb_to_yuv(unsigned int r, unsigned int g, unsigned int b, float *y, float *u, float *v);` | Converts RGB to YUV color space for 24-bit images.           |
| `void bmp24_equalize(t_bmp24 *img);`                                        | Performs histogram equalization on 24-bit BMP images.        |

### Input and error management

- **Input handling** is done via console menus. The program expects integer inputs for menu selections and parameter values (e.g., brightness offset, threshold), and will display an error message in case of invalid input.
- **Value clamping** is used extensively to keep pixel values within valid bounds (0 to 255).
- **Memory allocation errors** are checked, and if memory cannot be allocated, functions handle errors by aborting the current operation and displaying an error message.
- **File handling** includes checking for successful file open/read/write operations, with error messages on failure.

### Known issues

- Some performance limitations may occur on large images due to non-optimized loops.
- Minor rounding errors during RGB ↔ YUV conversions may slightly affect color accuracy.