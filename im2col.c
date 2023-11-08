#include "loadKernel.h"
#include "loadImage.h"
#include <stdlib.h>

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


int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Uso: %s <img> <kernel>\n", argv[0]);
        return 1;
    }

    int imageType = getImageType(argv[1]);
    Img* img;
    Grayscale* gs;

    switch (imageType) {
        case 0:
            img = loadRGBImage(argv[1]);
            break;
        case 1:
            gs = loadGrayscaleImage(argv[1]);
            break;
        default:
            printf("!=3 || !=1 \n");
            return 1;
    }


    if (imageType == 1) {
        if (saveRGBImage("resultado.png", img) != 0) {
            printf("RBG ok\n");
        } else {
            printf("Erro RGB\n");
        }
    } else if (imageType == 0) {
        if (saveGrayscaleImage("resultado.png", gs) != 0) {
            printf("Grayscale ok\n");
        } else {
            printf("Erro grayscale\n");
        }
    }
    
    int rows, cols;
    int **kernel = loadKernel(argv[2],&rows, &cols);
    for(int i = 0; i <rows; i++){
		for(int j = 0; j<cols; j++){
			printf("%d ", kernel[i][j]);
		}
		printf("\n");
	}
    return 0;
}

