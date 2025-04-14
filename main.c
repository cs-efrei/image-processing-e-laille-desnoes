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
            printf("Chemin du fichier : ");
            scanf("%s", &filename);
            img = bmp8_loadImage(filename);
        }
        else if (choice == 2 && img) {
            printf("Chemin du fichier :");
            scanf("%s", filename);
            bmp8_saveImage(filename, img);
        }
        else if (choice == 3 && img) {
            printf("1. Négatif\n 2. Luminosité \n3. Binarisation \n4. Flou \n5. Flou gaussien \n5. Netteté \n6. Contours \n7. Relief \n8. Retourner au menu précédent");
            scanf(" %d", &choice);
            switch (choice) {
                case 1:
                    bmp8_negative(t_bmp8 *img);
                case 2:
                    int value;
                    printf("Comment voulez vous changer la luminausité.");
                    scanf(" %d", &value);
                    bmp8_brightness(t_bmp8 *img, value);
                case 3:
                    int value;
                    printf("entrez un entier pour la binarisation");
                    scanf("%d", &value);
                    bmp8_threshold(t_bmp8 *img, value);
                case 4:
                    return;
            }
            bmp8_applyFilter(img);
        }
        else if (choice == 4 && img) {
            bmp8_saveImage(img);
        }
        else if (choice == 5 && img) {
            bmp8_free(img);
            return;
        }
        else {
            printf("Option invalide ou image non chargée.\n");
        }
    }
    return 0;
}


