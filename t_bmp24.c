//
// Created by marin on 15/05/2025.
//

#include "t_bmp24.h"

#include <stdio.h>
#include <stdlib.h>



#include <math.h> // utile que pour le round de la fonction applyfilter

t_pixel **bmp24_allocateDataPixels(int width, int height) {
    t_pixel **pixels = (t_pixel **)malloc(height * sizeof(t_pixel *));
    if (!pixels) return NULL;

    for (int i = 0; i < height; i++) {
        pixels[i] = (t_pixel *)malloc(width * sizeof(t_pixel));
        if (!pixels[i]) {
            printf("Ereur d'allocation de memoire\n");
            for (int j = 0; j < i; j++) {
                free(pixels[j]);
            }
            free(pixels);
            return NULL;
        }
    }
    return pixels;
}

void bmp24_freeDataPixels (t_pixel ** pixels, int height) {
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
}


t_bmp24 *bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *img = (t_bmp24 *)malloc(sizeof(t_bmp24));

    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;

    img->data = bmp24_allocateDataPixels(width, height);

    return img;
}


void bmp24_free(t_bmp24 *img) {
    bmp24_freeDataPixels(img->data, img->height);
    free(img);
}

void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    // int fseek( FILE * file, long position, int reference );
    // SEEK_SET car la position est exprimée à partir du début du fichier. La position doit donc être positive.
    // position : indique la position relativement par rapport au référentiel spécifié.
    fseek(file, position, SEEK_SET);
    // size_t Peut stocker la taille maximale d’un objet théoriquement possible de n’importe quel type
    // size_t fread( void * buffer, size_t blocSize, size_t blocCount, FILE * stream );
    // buffer : permet de spécifier le bloc de mémoire dans lequel stocker les octets à lire, cette taille = blocSize * blocCount.
    // stream : représente le flux à partir duquel lire les octets demandés
    // fread Renvoie le nombre de blocs lus
    fread(buffer, size, n, file);
}


void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
    fseek(file, position, SEEK_SET);
    // size_t fwrite( void * buffer, size_t blocSize, size_t blocCount, FILE * stream );
    // buffer : permet de spécifier le bloc de mémoire dans lequel stocker les octets à ecrire
    // stream : représente le flux à partir duquel ecrire les octets demandés
    // fread Renvoie le nombre de blocs ecris
    fwrite(buffer, size, n, file);
}

void bmp24_readPixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    uint32_t offset = BITMAP_OFFSET + (y * image->width + x) * 3; // Position pour un pixel
    uint8_t pixel[3]; // Stocke B, G, R
    file_rawRead(offset, pixel, sizeof(uint8_t), 3, file);
    image->data[y][x].blue = pixel[0];
    image->data[y][x].green = pixel[1];
    image->data[y][x].red = pixel[2];
}

void bmp24_writePixelValue(t_bmp24 *image, int x, int y, FILE *file) {
    uint32_t offset = BITMAP_OFFSET + (y * image->width + x) * 3; // Position pour un pixel
    uint8_t pixel[3]; // Stocke B, G, R
    image->data[y][x].blue = pixel[0];
    image->data[y][x].green = pixel[1];
    image->data[y][x].red = pixel[2];
    file_rawWrite(offset, pixel, sizeof(uint8_t), 3, file);
}

void bmp24_readPixelData(t_bmp24 *image, FILE *file) {
    int padding = (4 - (image->width * 3) % 4) % 4;

    for (int y = image->height - 1; y >= 0; y--) {
        for (int x = 0; x < image->width; x++) {
            bmp24_readPixelValue(image, x, y, file);
        }
        fseek(file, padding, SEEK_CUR); // ignorer le padding
    }
}

void bmp24_writePixelData(t_bmp24 *image, FILE *file) {
    int padding = (4 - (image->width * 3) % 4) % 4;
    uint8_t zero = 0;

    for (int y = image->height - 1; y >= 0; y--) {
        for (int x = 0; x < image->width; x++) {
            bmp24_writePixelValue(image, x, y, file);
        }
        for (int p = 0; p < padding; p++) {
            file_rawWrite(file, &zero, 1);
        }
    }
}

t_bmp24 *bmp24_loadImage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) return NULL;

    t_bmp_header header;
    file_rawRead(file, &header, sizeof(t_bmp_header));

    if (header.type != 0x4D42) {
        fclose(file);
        return NULL;
    }

    t_bmp_info info;
    file_rawRead(file, &info, sizeof(t_bmp_info));

    if (info.bits != 24 || info.compression != 0) {
        fclose(file);
        return NULL;
    }

    t_bmp24 *img = bmp24_allocate(info.width, info.height, 24);
    if (!img) {
        fclose(file);
        return NULL;
    }

    img->header = header;
    img->header_info = info;

    fseek(file, header.offset, SEEK_SET);
    bmp24_readPixelData(img, file);

    fclose(file);
    return img;
}


void bmp24_saveImage(t_bmp24 *img, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (!file) return;

    int padding = (4 - (img->width * 3) % 4) % 4;
    int rowSize = img->width * 3 + padding;
    int imageSize = rowSize * img->height;

    // Remplir les en-têtes
    img->header.type = 0x4D42;
    img->header.size = 54 + imageSize;
    img->header.offset = 54;
    img->header.reserved1 = 0;
    img->header.reserved2 = 0;

    img->header_info.size = 40;
    img->header_info.width = img->width;
    img->header_info.height = img->height;
    img->header_info.planes = 1;
    img->header_info.bits = 24;
    img->header_info.compression = 0;
    img->header_info.imagesize = imageSize;
    img->header_info.xresolution = 3780;
    img->header_info.yresolution = 3780;
    img->header_info.ncolors = 0;
    img->header_info.importantcolors = 0;

    file_rawWrite(file, &img->header, sizeof(t_bmp_header));
    file_rawWrite(file, &img->header_info, sizeof(t_bmp_info));
    bmp24_writePixelData(img, file);

    fclose(file);
}



// Fonction pour inverser les couleurs de l'image
void bmp24_negative(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            img->data[y][x].red   = 255 - img->data[y][x].red;
            img->data[y][x].green = 255 - img->data[y][x].green;
            img->data[y][x].blue  = 255 - img->data[y][x].blue;
        }
    }
}

void bmp24_grayscale(t_bmp24 *img) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            uint8_t r = img->data[y][x].red;
            uint8_t g = img->data[y][x].green;
            uint8_t b = img->data[y][x].blue;
            uint8_t gray = (r + g + b) / 3;
            img->data[y][x].red = gray;
            img->data[y][x].green = gray;
            img->data[y][x].blue = gray;
        }
    }
}


void bmp24_brightness(t_bmp24 *img, int value) {
    for (int y = 0; y < img->height; y++) {
        for (int x = 0; x < img->width; x++) {
            int r = img->data[y][x].red + value;
            int g = img->data[y][x].green + value;
            int b = img->data[y][x].blue + value;

            img->data[y][x].red   = (uint8_t)(fminf(255, fmaxf(0, r)));
            img->data[y][x].green = (uint8_t)(fminf(255, fmaxf(0, g)));
            img->data[y][x].blue  = (uint8_t)(fminf(255, fmaxf(0, b)));
        }
    }
}

