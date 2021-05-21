/* ////////////////////////////////////////////////////////////////////////
* ////////////////////      Fichier : main.c            ///////////////////
* ////////////////////      Auteur : Youssef HAMDOUNI   ///////////////////
* ///////////////////////////////////////////////////////////////////////// */

#include <stdio.h>
#include <time.h>

#include "ImageGris.h"
#include "PairUIntMap_u8.h"
#include "Log.h"

#define SOURCE_ASSETS(file) file
#define DESTINATION_ASSETS(file) file

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        fprintf(stderr, "usage: %s <image> <marqueur> <rayon>\n", argv[0]);

        return EXIT_FAILURE;
    }

    srand(time(NULL));

    ImageGris *img = LireImage(argv[1]);
    ImageGris *marqueur = LireImage(argv[2]);
    ImageGris *gradient = CalculerGradient(img, atoi(argv[3]));
    ImageGris *lpe = CalculerLPE(gradient, marqueur);
    EcrireImage(img, DESTINATION_ASSETS("copie.png"));
    EcrireImage(gradient, DESTINATION_ASSETS("gradient_out.png"));
    EcrireImage(lpe, DESTINATION_ASSETS("lpe_out.png"));
    EcrireImage(marqueur, DESTINATION_ASSETS("marqueur_copie.png"));
    EcrireLPECouleur(lpe, DESTINATION_ASSETS("lpe_col_out.png"));

    RossortirContour(lpe, argv[1], DESTINATION_ASSETS("contours.png"));

    LibererImage(img);
    LibererImage(marqueur);
    LibererImage(gradient);
    LibererImage(lpe); 

    return EXIT_SUCCESS;
}