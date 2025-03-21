#include "bmp8.h"

t_bmp8 * bmp8_loadImage(const char * filename) {
  /* Truc nul à corriger */
  FILE *f = fopen(filename, "rb");
  unsigned char header[54];
  fread(header, 1, 54, f);
  int width = *(unsigned int *)&(header[18]);
  int height = *(unsigned int *)&(header[22]);
  int colorDepth = *(unsigned int *)&(header[24]);
  int dataSize = *(unsigned int *)&(header[28]);
  char colorTable[1024] ;
  fread(&colorDepth, 1, 1, f);
  fread(&width, 1, 1, f);
  fread(&height, 1, 1, f);
  fread(&width, 1, 1, f);
  fread(&height, 1, 1, f);
  fread(&colorDepth, 1, 1, f);
  fread(&colorTable, 1, 1, f);
  t_bmp8 * bmp8 = new t_bmp8;
  bmp8->header = *header;
  bmp8->width = width;
  bmp8->height = height;
  bmp8->colorDepth = colorDepth;
  bmp8->dataSize = dataSize;
  bmp8->colorTable = *colorTable;
  bmp8->data = (unsigned char *) malloc(bmp8->dataSize);
  return bmp8;
}