// main.c - Interface en ligne de commande
#include "t_bmp8.h"



int main() {
    const char filename[256];
    int choice;
    t_bmp8 *img = NULL;

    while (1) {
        printf("\n1. Ouvrir une image\n2. Sauvegarder une image\n3. Appliquer un filtre\n4. Afficher les informations de l'image\n5. Quitter\nVotre choix : ");
        scanf(" %d", &choice);

        if (choice == 1) {
            printf("Chemin du fichier :");
            scanf("%s", &filename);
            img = bmp8_loadImage(filename);
        }
        else if (choice == 2 && img) {
            printf("Chemin du fichier : ");
            scanf("%s", filename);
            bmp8_saveImage(filename, img);
        }
        else if (choice == 3 && img) {
            int filtre;
            printf("1. negatif\n 2.Luminosite \n3. Binarisation \n4. Flou \n5. Flou gaussien \n6. Nettete \n7. Contours \n8. Relief \n9. Retourner au menu precedent \nVotre choix : ");
            scanf(" %d", &filtre);
            int value, kernelSize = 3;
            if (filtre==1) {
                    bmp8_negative(img);
                }
            else if (filtre==2){
                printf("Comment voulez vous changer la luminausité : ");
                scanf(" %d", &value);
                bmp8_brightness(img, value);
                }
            else if (filtre==3){
                printf("Entrez un entier pour la binarisation : ");
                scanf("%d", &value);
                bmp8_threshold(img, value);
            }
            else if (filtre==4) {
                float blox_blur[3][3] = {{1.0/9,1.0/9,1.0/9},{1.0/9,1.0/9,1.0/9},{1.0/9,1.0/9,1.0/9}};
                bmp8_applyFilter(img, blox_blur, kernelSize);
            }
            else if (filtre==5) {
                float gaussian_blur[3][3] = {{1.0/16,1.0/8,1.0/16},{1.0/8,1.0/4,1.0/8},{1.0/16,1.0/8,1.0/16}};
                bmp8_applyFilter(img, gaussian_blur, kernelSize);
            }
            else if (filtre==6) {
                float outline[3][3] = {{-1.0,-1.0,-1.0},{-1.0,8.0,-1.0},{-1.0,-1.0,-1.0}};
                bmp8_applyFilter(img, outline, kernelSize);
            }
            else if (filtre==7) {
                float emboss[3][3] = {{-2.0,-1.0,0},{-1.0,1.0,1.0},{0,1.0,2.0}};
                bmp8_applyFilter(img, emboss, kernelSize);
            }
            else if (filtre==8) {
                float sharpen[3][3] = {{0,-1.0,0},{-1.0,5.0,-1.0},{0,-1.0,0}};
                bmp8_applyFilter(img, sharpen, kernelSize);
            }
            else {
                printf("Votre choix : ");
            }
        }
        else if (choice == 4 && img) {
            bmp8_printInfo(img);
        }
        else if (choice == 5) {
            bmp8_free(img);
            return 0;
        }
        else {
            printf("Option invalide ou image non chargée.\n");
        }
    }
    return 0;
}


