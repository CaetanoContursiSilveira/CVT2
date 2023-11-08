#ifndef LOAD_IMAGE_H
#define LOAD_IMAGE_H

#include <math.h>
//Imagens com 3 canais de cores, pra ter somente uma matriz
typedef struct {
    unsigned char r, g, b;
} Pixel;

typedef struct {
    int height, width;
    Pixel** data;
} Img;
//k o unico canal de cor para grayscale
typedef struct {
    int height, width;
    unsigned char** k;
} Grayscale;


int getImageType(const char* filename);
Img* loadRGBImage(const char* filename);
Grayscale* loadGrayscaleImage(const char* filename);
int saveRGBImage(const char* filename, Img* img);
int saveGrayscaleImage(const char* filename, Grayscale* grayscale);

#endif 

