// bmp8.c - Implémentation des fonctions
#include "t_bmp8.h"
#include <stdio.h>
#include <stdlib.h>

t_bmp8* bmp8_loadImage(const char *filename) {
    // permet de lire une image en niveaux de gris à partir d’un fichier BMP, filename
    FILE *file = fopen(filename, "rb"); //Ouvre le fichier en mode binaire
    // FILE est une structure qui représente un fichier ouvert en C. Elle est définie dans la bibliothèque <stdio.h>.
    if (!file) { //file vaut NULL si l'ouverture du fichier échoue alors sir l'ouverture echoue, !file vaut vrai
        printf("Erreur : Impossible d'ouvrir le fichier %s\n", filename);
        return NULL;
    }

    t_bmp8 *img = (t_bmp8 *)malloc(sizeof(t_bmp8));
    fread(img->header, sizeof(unsigned char), 54, file); // lecture dde l'en-tête BMP
    // Quand on lit un fichier binaire, chaque octet est traité comme une séquence de 8 bits.
    // En utilisant sizeof(unsigned char), on s'assure que la lecture est faite octet par octet.
    // On pourrait aussi écrire fread(header, 1, 54, file);
    img->width = *(int*)&img->header[18];
    img->height = *(int*)&img->header[22];
    img->dataSize = img->width * img->height;
    fread(img->colorTable, sizeof(unsigned char), 1024, file);
    img->data = (unsigned char *)malloc(img->dataSize);
    fread(img->data, sizeof(unsigned char), img->dataSize, file);
    fclose(file); // ferme le fichier
    return img;
}

void bmp8_saveImage(const char *filename, t_bmp8 *img) {
    FILE *file = fopen(filename, "wb");
    fwrite(img->header, sizeof(unsigned char), 54, file);
    fwrite(img->colorTable, sizeof(unsigned char), 1024, file);
    fwrite(img->data, sizeof(unsigned char), img->dataSize, file);
    fclose(file);
}

void bmp8_free(t_bmp8 *img) {
    // libére la mémoire allouée pour stocker une image img
    free(img->data);
    free(img);
}

void bmp8_printInfo(t_bmp8 *img) {
    printf("Width: %d\n", img->width);
    printf("Height: %d\n", img->height);
    printf("Data Size: %d\n", img->dataSize);
}
/*
void bmp8_negative(t_bmp8 *img) {
    for (int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
}

void bmp8_brightness(t_bmp8 *img, int value) {
    for (int i = 0; i < img->dataSize; i++) {
        int temp = img->data[i] + value;
        img->data[i] = (temp > 255) ? 255 : (temp < 0) ? 0 : temp;
    }
}

void bmp8_threshold(t_bmp8 *img, int threshold) {
    for (int i = 0; i < img->dataSize; i++) {
        img->data[i] = (img->data[i] >= threshold) ? 255 : 0;
    }
}

void bmp8_applyFilter(t_bmp8 img, float kernel[3][3]) {
    unsigned charnewData = (unsigned char )malloc(img->dataSize);
    for (int y = 1; y < img->height - 1; y++) {
        for (int x = 1; x < img->width - 1; x++) {
            float sum = 0;
            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    sum += img->data[(y + j) img->width + (x + i)] * kernel[j + 1][i + 1];
                }
            }
            newData[y * img->width + x] = (sum > 255) ? 255 : (sum < 0) ? 0 : (unsigned char)sum;
        }
    }
    free(img->data);
    img->data = newData;
}
*/

