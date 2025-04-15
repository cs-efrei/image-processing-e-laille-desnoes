// bmp8.c - Implémentation des fonctions
#include "t_bmp8.h"
#include <stdio.h>
#include <stdlib.h>

#include <math.h> // utile que pour le round de la fonction applyfilter

t_bmp8* bmp8_loadImage(const char *filename) {
    // permet de lire une image en niveaux de gris à partir d’un fichier BMP, filename
    FILE *file = NULL;
    file = fopen(filename, "rb"); //Ouvre le fichier en mode binaire
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
    // ferme le fichier
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
    // libére la mémoire allouée pour stocker une image img
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

    // Vérifie que img->data existe
    if (img->data == NULL) {
        printf("Erreur : les données de l'image sont nulles\n");
        return;
    }

    // Vérifie que la taille est correcte
    if (img->dataSize != width * height) {
        printf("Alerte : img->dataSize incorrecte (%u au lieu de %u)\n", img->dataSize, width * height);
        img->dataSize = width * height;
    }

    // Alloue un tableau temporaire
    unsigned char *newData = (unsigned char *)malloc(img->dataSize);
    if (!newData) {
        printf("Erreur malloc\n");
        return;
    }

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

            // Clamp
            if (sum > 255) sum = 255;
            if (sum < 0) sum = 0;

            newData[y * width + x] = (unsigned char)roundf(sum);
        }
    }

    // Remplace les données
    free(img->data);
    img->data = newData;

    printf("Filtre appliqué avec succès ✅\n");
}