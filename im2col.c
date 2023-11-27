#include "loadKernel.h"
#include "loadImage.h"
#include <stdlib.h>

Pixel resultPixel(int* kernel, Pixel* pixels, int sum, int size) {
    Pixel result = {0, 0, 0};

    for (int i = 0; i < size; i++) {
        result.r += kernel[i] * pixels[i].r;
        result.g += kernel[i] * pixels[i].g;
        result.b += kernel[i] * pixels[i].b;
    }
    result.r = (result.r / sum);
    result.g = (result.g / sum);
    result.b = (result.b / sum);
	if (result.r > 255) result.r = 255;
	if (result.g > 255) result.g = 255;
	if (result.b > 255) result.b = 255;
	if (result.r < 0) result.r = 0;
	if (result.g < 0) result.g = 0;
	if (result.b < 0) result.b = 0;
	fflush(stdout);
	printf("%d", result.r);
	printf("%d", result.g);
	printf("%d", result.b);
	result.r = (unsigned char)result.r;
	result.g = (unsigned char)result.g;
	result.b = (unsigned char)result.b;

    return result;
}


Pixel* pixelToCol(Pixel** matrix, int x, int y, int height, int width) {
    Pixel* column = (Pixel*)malloc(width * height * sizeof(Pixel));
    int col_index = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixel_x = x + i;
            int pixel_y = y + j;

            if (pixel_x >= 0 && pixel_x < height && pixel_y >= 0 && pixel_y < width) {
                column[col_index] = matrix[pixel_x][pixel_y];
            } else {
                free(column); 
                return NULL;
            }

            col_index++;
        }
    }

    return column; 
}

Img* resultImage(Img* in, int* kernel, int sum, int ksize,int klength){
	int x,y;
	x = in->width - 2*(ksize -2);
	y = in->height - 2*(ksize -2);
	Img* out = (Img*)malloc(x * y * sizeof(Img));
	out->width = x;
	out->height = y;
	out->data = (Pixel**)malloc(y * sizeof(Pixel*));
	for(int i = 0; i < y; i++){
		out->data[i] = (Pixel*)malloc(x * sizeof(Pixel));
		for(int j = 0; j < x; j++){
			Pixel* column = pixelToCol(in->data, i, j, ksize, ksize);
			 if (column != NULL) {//erro ta aqui
                Pixel result = resultPixel(kernel, column, sum, klength);
                out->data[i][j] = result;
                free(column);
            } else {
                free(out->data);
                free(out);
                return NULL;
            }
		}
	}
	return out;
}

unsigned char* constToCol(unsigned char** matrix, int x, int y, int height, int width) {
    unsigned char* column = (unsigned char*)malloc(width * height * sizeof(unsigned char));
    int col_index = 0;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int pixel_x = x + i;
            int pixel_y = y + j;

            if (pixel_x >= 0 && pixel_x < height && pixel_y >= 0 && pixel_y < width) {
                column[col_index] = matrix[pixel_x][pixel_y];
            } else {
                free(column); 
                return NULL;
            }

            col_index++;
        }
    }

    return column; 
}


int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Uso: %s <img> <kernel> <N para dividir o pixel> <1 Para pedding>\n", argv[0]);
        return 1;
    }
    
    int imageType = getImageType(argv[1]);
    Img* img;
    Grayscale* gs;
    /////////////KERNEL//////////////////////
    int rows, cols,sum, k;
    int **kernel0 = loadKernel(argv[2],&rows, &cols);
    if(rows != cols){
		printf("KERNEL INVALIDO!");	
		return 1;
	}
	sum = atoi(argv[3]);
    int *kernel = (int*)malloc(rows * cols * sizeof(int));
    int n = cols - 1 - (cols % 2);
	k = 0; 
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			kernel[k] = kernel0[i][j];
			k++;
    }
    printf("\n");
	}
	int size = cols * cols;
	int padding = atoi(argv[4]);
	///////////////////////////////////////////
	
	if(imageType == 3){
		if(padding == 1){
			img = RGBPadding1(argv[1],n);
			Img* im = loadRGBImage(argv[1]);
			if(saveRGBImage("padding.png", img)){
				printf("Padding ok\n");
			}
			Img* i2 = resultImage(im, kernel, sum, cols, k);
			if (saveRGBImage("resultado.png", i2) != 0) {
				printf("RBG ok\n");
			} else {
				printf("Erro RGB\n");
			}	
		}else{
			img = loadRGBImage(argv[1]);
		}
	}	
    return 0;
}

