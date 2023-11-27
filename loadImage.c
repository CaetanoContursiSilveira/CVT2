#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "loadImage.h"
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"
int getImageType(const char* filename) {
    int width, height, channels;
    unsigned char* imgData = stbi_load(filename, &width, &height, &channels,0);
    return channels;
}

Img* loadRGBImage(const char* filename) {
    int width, height, channels;
    unsigned char* imgData = stbi_load(filename, &width, &height, &channels, STBI_rgb);

    if (channels != 3) {
        return NULL;
    }

    if (imgData) {
        Img* img = (Img*)malloc(sizeof(Img));
        img->width = width;
        img->height = height;
        img->data = (Pixel**)malloc(height * sizeof(Pixel*));

        for (int i = 0; i < height; i++) {
            img->data[i] = (Pixel*)malloc(width * sizeof(Pixel));
            for (int j = 0; j < width; j++) {
                int offset = (i * width + j) * 3;
                img->data[i][j].r = imgData[offset];
                img->data[i][j].g = imgData[offset + 1];
                img->data[i][j].b = imgData[offset + 2];
            }
        }

        stbi_image_free(imgData);
        return img;
    }

    return NULL;
}
Img* RGBPadding1(const char* filename, int n) {
    int width, height, channels;
    unsigned char* imgData = stbi_load(filename, &width, &height, &channels, STBI_rgb);

    if (channels != 3) {
        return NULL;
    }

    Img* img = (Img*)malloc(sizeof(Img));
    img->width = width + 2 * n;
    img->height = height + 2 * n;
    img->data = (Pixel**)calloc(img->height, sizeof(Pixel*));

    for (int i = 0; i < img->height; i++) {
        img->data[i] = (Pixel*)calloc(img->width, sizeof(Pixel));

        for (int j = 0; j < img->width; j++) {
            if (i >= n && i < height + n && j >= n && j < width + n) {
                int offset = (i * width + j) * 3;
                img->data[i][j] = (Pixel){imgData[offset], imgData[offset + 1], imgData[offset + 2]};
            } else {
                img->data[i][j] = (Pixel){0, 0, 0};
            }
        }
    }

    stbi_image_free(imgData);
    return img;
}


Grayscale* GrayscalePadding1(const char* filename, int n) {
    int width, height, channels;
    unsigned char* imgData = stbi_load(filename, &width, &height, &channels, STBI_grey);

    if (channels != 1) {
        return NULL;
    }

    if (imgData) {
        Grayscale* grayscale = (Grayscale*)malloc(sizeof(Grayscale));
        grayscale->width = width;
        grayscale->height = height;
        grayscale->k = (unsigned char**)calloc((height + 2 * n), sizeof(unsigned char*));

        for (int i = 0; i < height + 2 * n; i++) {
            grayscale->k[i] = (unsigned char*)calloc((width + 2 * n), sizeof(unsigned char));
        }

        for (int i = n; i < height + n; i++) {
			for (int j = n; j < width + n; j++) {
				int offset = (i - n) * width + (j - n);
                grayscale->k[i][j] = imgData[offset];
            }
        }
        stbi_image_free(imgData);
        return grayscale;
    }

    return NULL;
}

Grayscale* loadGrayscaleImage(const char* filename) {
    int width, height, channels;
    unsigned char* imgData = stbi_load(filename, &width, &height, &channels, STBI_grey);

    if (channels != 1) {
        return NULL;
    }

    if (imgData) {
        Grayscale* grayscale = (Grayscale*)malloc(sizeof(Grayscale));
        grayscale->width = width;
        grayscale->height = height;
        grayscale->k = (unsigned char**)malloc(height * sizeof(unsigned char*));

        for (int i = 0; i < height; i++) {
            grayscale->k[i] = (unsigned char*)malloc(width * sizeof(unsigned char));
            for (int j = 0; j < width; j++) {
                int offset = i * width + j;
                grayscale->k[i][j] = imgData[offset];
            }
        }

        stbi_image_free(imgData);
        return grayscale;
    }

    return NULL;
}

int saveRGBImage(const char* filename, Img* img) {
    if (img == NULL) {
        return 0;
    }
    
    int channels = 3;
    unsigned char* imgData = (unsigned char*)malloc(img->width * img->height * channels);
    for (int i = 0; i < img->height; i++) {
        for (int j = 0; j < img->width; j++) {
            int offset = (i * img->width + j) * channels;
            imgData[offset] = img->data[i][j].r;
            imgData[offset + 1] = img->data[i][j].g;
            imgData[offset + 2] = img->data[i][j].b;
        }
    }
    int result = stbi_write_png(filename, img->width, img->height, channels, imgData, img->width * channels);
    free(imgData);
    return result;
}

int saveGrayscaleImage(const char* filename, Grayscale* grayscale) {
    if (grayscale == NULL) {
        return 0;
    }

    int channels = 1;
    unsigned char* imgData = (unsigned char*)malloc(grayscale->width * grayscale->height);

    for (int i = 0; i < grayscale->height; i++) {
        for (int j = 0; j < grayscale->width; j++) {
            int offset = i * grayscale->width + j;
            imgData[offset] = grayscale->k[i][j];
        }
    }

    int result = stbi_write_png(filename, grayscale->width, grayscale->height, channels, imgData, grayscale->width * channels);
    free(imgData);

    return result;
}
