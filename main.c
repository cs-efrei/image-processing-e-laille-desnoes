// main.c - Interface en ligne de commande
#include "t_bmp8.h"

int main() {
    char filename[256];
    int choice;
    t_bmp8 *img = NULL;

    while (1) {
        printf("\n1. Charger une image\n2. Sauvegarder une image\n3. Appliquer un négatif\n4. Modifier la luminosité\n5. Seuillage\n6. Quitter\nVotre choix : ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Entrez le nom du fichier : ");
            scanf("%s", filename);
            img = bmp8_loadImage(filename);
        } else if (choice == 2 && img) {
            printf("Nom du fichier de sortie : ");
            scanf("%s", filename);
            bmp8_saveImage(filename, img);
        } else if (choice == 3 && img) {
            bmp8_negative(img);
        } else if (choice == 4 && img) {
            int value;
            printf("Valeur de luminosité (+/-) : ");
            scanf("%d", &value);
            bmp8_brightness(img, value);
        } else if (choice == 5 && img) {
            int threshold;
            printf("Valeur de seuil : ");
            scanf("%d", &threshold);
            bmp8_threshold(img, threshold);
        } else if (choice == 6) {
            if (img) bmp8_free(img);
            break;
        } else {
            printf("Option invalide ou image non chargée.\n");
        }
    }
    return 0;
}


