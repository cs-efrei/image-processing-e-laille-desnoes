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
            bmp8_applyFilter(img);
        }
        else if (choice == 4 && img) {
            bmp8_saveImage(img);
        }
        else if (choice == 5 && img) {
            bmp8_free(img);
        }
        else {
            printf("Option invalide ou image non chargée.\n");
        }
    }
    return 0;
}


