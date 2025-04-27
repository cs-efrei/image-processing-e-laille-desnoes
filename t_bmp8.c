// bmp8.c - Implémentation des fonctions
#include "t_bmp8.h"
#include <stdio.h>
#include <stdlib.h>

#include <math.h> // utile que pour le round de la fonction applyfilter

t_bmp8* bmp8_loadImage(const char *filename) {
    FILE *file = NULL;
    file = fopen(filename, "rb");
    if (!file) {
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        return NULL;
    }
    t_bmp8 *img = (t_bmp8 *)malloc(sizeof(t_bmp8));
    fread(img->header, sizeof(unsigned char), 54, file);
    img->width = *(int*)&img->header[18];
    img->height = *(int*)&img->header[22];
    img->dataSize = img->width * img->height;
    fread(img->colorTable, sizeof(unsigned char), 1024, file);
    img->data = (unsigned char *)malloc(img->dataSize);
    fread(img->data, sizeof(unsigned char), img->dataSize, file);
    printf("Image chargee avec succes !");
    return img;
}

void bmp8_saveImage(const char *filename, t_bmp8 *img) {
    FILE *file = fopen(filename, "wb");
    fwrite(img->header, sizeof(unsigned char), 54, file);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);
    fclose(file);
    printf("Image sauvegardée avec succes !");
}

void bmp8_free(t_bmp8 *img) {
    free(img->data);
    free(img);
}

void bmp8_printInfo(t_bmp8 *img) {
    printf("Image Info\n");
    printf("    Width: %d\n", img->width);
    printf("    Height: %d\n", img->height);
    printf("    Color Depth: %d\n", img->colorDepth);
    printf("    Data Size: %d\n", img->dataSize);
}

// Fonction pour inverser les couleurs de l'image
void bmp8_negative(t_bmp8 *img) {
    for (int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
}

void bmp8_brightness(t_bmp8 *img, int value) {
    for (int i = 0; i < img->dataSize; i++) {
        int temp = img->data[i] + value;
        if (temp > 255) {
            temp = 255;
        }
        else if (temp < 0) {
            temp = 0;
        }
        img->data[i] = temp;
    }
}

void bmp8_threshold(t_bmp8 *img, int threshold) {
    for (int i = 0; i < img->dataSize; i++) {
        if (img->data[i] >= threshold) {
            img->data[i] = 255;
        }
        else {
            img->data[i] = 0;
        }
    }
}



void bmp8_applyFilter(t_bmp8 *img, float kernel[3][3], int kernelSize) {
    unsigned int width = img->width;
    unsigned int height = img->height;
    int offset = kernelSize / 2;

    unsigned char *newData = (unsigned char *)malloc(img->dataSize);

    for (unsigned int y = 0; y < height; y++) {
        for (unsigned int x = 0; x < width; x++) {
            float sum = 0.0f;

            for (int i = -offset; i <= offset; i++) {
                for (int j = -offset; j <= offset; j++) {
                    int py = y + i;
                    int px = x + j;

                    if (px >= 0 && px < (int)width && py >= 0 && py < (int)height) {
                        unsigned char pixel = img->data[py * width + px];
                        float coeff = kernel[i + offset][j + offset];
                        sum += pixel * coeff;
                    }
                }
            }

            if (sum > 255) sum = 255;
            if (sum < 0) sum = 0;

            newData[y * width + x] = (unsigned char)roundf(sum);
        }
    }
    free(img->data);
    img->data = newData;
}