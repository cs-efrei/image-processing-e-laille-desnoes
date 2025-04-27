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

void bmp8_applyFilter(t_bmp8 * img, float kernel[3][3], int kernelSize);

// Partie 2 :

typedef struct {
    t_bmp_header header;
    t_bmp_info header_info;
    int width;
    int height;
    int colorDepth;
    t_pixel **data;
} t_bmp24;

typedef struct {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
} t_bmp_header;

typedef struct {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits;
    uint32_t compression;
    uint32_t imagesize;
    int32_t xresolution;
    int32_t yresolution;
    uint32_t ncolors;
    uint32_t importantcolors;
} t_bmp_info;

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} t_pixel;


#endif


