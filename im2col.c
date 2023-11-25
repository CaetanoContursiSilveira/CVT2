#include "loadKernel.h"
#include "loadImage.h"
#include <stdlib.h>



Pixel resultPixel(int* kernel, Pixel* pixels, int sum, int size) {
    Pixel result = {0, 0, 0};

    for (int i = 0; i < size; i++) {
        result.r += (int)kernel[i] * pixels[i].r;
        result.g += (int)kernel[i] * pixels[i].g;
        result.b += (int)kernel[i] * pixels[i].b;
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

Img* resultImage(Img* in, int* kernel, int sum, int size, int cols) {
    Img* out = (Img*)malloc(sizeof(Img));
    out->width = in->width;
    out->height = in->height;
    out->data = (Pixel**)malloc(out->height * sizeof(Pixel*));

    for (int i = 0; i < out->height; i++) {
        out->data[i] = (Pixel*)malloc(out->width * sizeof(Pixel));
        for (int j = 0; j < out->width; j++) {
            Pixel* buffer = pixelToCol(in->data, i, j, cols, cols);
            Pixel result = resultPixel(kernel, buffer, sum, size);
            out->data[i][j] = result;
            free(buffer); 
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
	int col = cols;
	int padding = atoi(argv[4]);
	///////////////////////////////////////////
	
	if(imageType == 3){
		if(padding = 1){
			img = RGBPadding0(argv[1],col -1);
			Img* imgOut = resultImage(img, kernel, sum,k,cols);
			if (saveRGBImage("resultado.png", imgOut) != 0) {
				printf("RBG ok\n");
			} else {
				printf("Erro RGB\n");
			}	
		}
	}	
    return 0;
}

