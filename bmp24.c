#include "bmp24.h"
int limit(int a) {
    /*This function caps the input pixels values in case they exceed the values
     Return the closest bound
     */
    if (a > 255) {
        return  255;
    }
    if (a < 0) {
        return 0;
    }
    return a;
}

void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    /*Enables to read a file at a set position*/
    fseek(file, position, SEEK_SET);
    fread(buffer, size, n, file);
}

void file_rawWrite (uint32_t position, void* buffer, uint32_t size, size_t n, FILE * file) {
    /*Enables to write a file at a set position*/
    fseek(file, position, SEEK_SET);
    fwrite(buffer, size, n, file);
}


void reverse_rows(t_pixel** M, int width, int height) {
    /*Function used to reverse the matrix M of size width x height of
     pixels after reading and writing*/
    t_pixel** A = bmp24_allocateDataPixels(width,height);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            A[i][j] = M[i][j];
        }
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            M[i][j] = A[height-i-1][j];
        }
    }
}


t_pixel** bmp24_allocateDataPixels(int width, int height) {
    //Same as YUV, but for RGB
    t_pixel** matrix = (t_pixel**)malloc(sizeof(t_pixel*)*height);
    if (!matrix) {
        printf("Error in allocation for the matrix!\n");
        return NULL;
    }
    for (int i = 0; i<height; i++) {
        t_pixel* row = (t_pixel*)malloc(sizeof(t_pixel)*width);
        if (!row) {
            printf("Error in allocation for the pixels!\n");
            free(matrix);
            return NULL;
        }
        matrix[i] = row;
    }
    return matrix;
}

void bmp24_freeDataPixels(t_pixel ** pixels, int height) {
    for (int i = 0; i<height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}


t_bmp24* bmp24_allocate(int width, int height, int colorDepth) {
    //Creates dynamically a bmp24 structure to store an image
    t_bmp24* image = (t_bmp24*)malloc(sizeof(t_bmp24));
    if (!image) {
        printf("Error in allocation for the image!\n");
        return NULL;
    }
    image -> width = width;
    image -> height = height;
    image -> colorDepth = colorDepth;
    image -> data = bmp24_allocateDataPixels(image -> width, image -> height);
    if (!(image -> data)) {
        free(image);
        return NULL;
    }
    return image;
}


void bmp24_free(t_bmp24 * img) {
    //Free the image bmp24
    bmp24_freeDataPixels(img -> data, img -> height);
    free(img);
}

t_bmp24* bmp24_loadImage(const char * filename) {
    //Creates the image, and stores its information
    FILE* f;
    f = fopen(filename, "rb");
    if (f == NULL){printf("Error while reading the file!\n");return NULL;}

    int width,height,colorDepth; // Parameters for the dynamic allocation
    file_rawRead(BITMAP_WIDTH,&width,sizeof(int),1,f);
    file_rawRead(BITMAP_HEIGHT,&height,sizeof(int),1,f);
    file_rawRead(BITMAP_DEPTH,&colorDepth,sizeof(int),1,f);
    if (colorDepth != DEFAULT_DEPTH) {
        printf("This image is not 24 bits deep!");
        fclose(f);
        return NULL;
    }
    t_bmp24* image = bmp24_allocate(width,height,colorDepth);
    if (!image) {
        fclose(f);
        return NULL;
    }
    t_bmp_header header;
    t_bmp_info header_info;
    file_rawRead(BITMAP_MAGIC, &header, HEADER_SIZE, 1, f); // Header of the file
    file_rawRead(HEADER_SIZE,&header_info,INFO_SIZE,1,f);
    image -> header = header;
    image -> header_info = header_info;
    bmp24_readPixelData(image,f);
    fclose(f);
    return image;
}


void bmp24_readPixelValue(t_bmp24* image, int x, int y, FILE * file) {
    //Reads the values of a single pixel
    int stride = ((image->width * 3 + 3) / 4) * 4;
    file_rawRead(HEADER_SIZE + INFO_SIZE + x * stride + y * 3,&(image -> data[x][y]),3,1,file);
    uint8_t tmp = (image -> data[x][y]).red;
    (image -> data[x][y]).red = (image -> data[x][y]).blue;
    (image -> data[x][y]).blue = tmp;
}

void bmp24_readPixelData(t_bmp24* image, FILE* file) {
    //Reads the values of the whole matrix
    for (int i=0; i < (image -> height) ; i++) {
        for (int j = 0; j<image->width; j++) {
            bmp24_readPixelValue(image, i, j, file);
        }
    }
    reverse_rows(image -> data, image -> width, image -> height);
}

void bmp24_writePixelValue(t_bmp24* image, int x, int y, FILE * file) {
    //Writes for one pixel
    uint8_t tmp = (image -> data[x][y]).red;
    (image -> data[x][y]).red = (image -> data[x][y]).blue;
    (image -> data[x][y]).blue = tmp;
    int stride = ((image->width * 3 + 3) / 4) * 4;
    file_rawWrite(HEADER_SIZE + INFO_SIZE + x * stride + y * 3,&(image -> data[x][y]),3,1,file);
}

void bmp24_writePixelData(t_bmp24* image, FILE* file) {
    //Writes for every pixels
    reverse_rows(image -> data, image -> width, image -> height);
    for (int i=0; i < (image -> height); i++) { //we begin at last row and go up
        for (int j = 0; j<image->width; j++) {
            bmp24_writePixelValue(image, i, j, file);
        }
    }
}


void bmp24_saveImage(t_bmp24* img, const char* filename) {
    //Saves the image by writing it under the name filename
    FILE* f;
    f = fopen(filename, "wb");
    if (f == NULL){printf("Error while reading the file!\n");return;}
    file_rawWrite(BITMAP_MAGIC,&(img -> header),HEADER_SIZE,1,f);
    file_rawWrite(HEADER_SIZE,&(img -> header_info),INFO_SIZE,1,f);
    bmp24_writePixelData(img,f);
    fclose(f);
}