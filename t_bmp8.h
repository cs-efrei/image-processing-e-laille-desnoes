// bmp8.h - Définition des structures et prototypes des fonctions
#ifndef BMP8_H
#define BMP8_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct {
    unsigned char header[54]; // représente l’en-tête du fichier BMP
    unsigned char colorTable[1024]; //  représente la table de couleur de l’image.
    unsigned char *data; // données
    unsigned int width;// la largeur de l’image en pixels
    unsigned int height; //  la hauteur de l’image en pixels
    unsigned int colorDepth;
    unsigned int dataSize; // taille des données de l’image en octets
} t_bmp8;

t_bmp8* bmp8_loadImage(const char *filename);
void bmp8_saveImage(const char *filename, t_bmp8 *img);
void bmp8_free(t_bmp8 *img);
void bmp8_printInfo(t_bmp8 *img);

void bmp8_negative(t_bmp8 *img);
void bmp8_brightness(t_bmp8 *img, int value);
void bmp8_threshold(t_bmp8 *img, int threshold);

void bmp8_applyFilter(t_bmp8 * img, float ** kernel, int kernelSize);

#endif

