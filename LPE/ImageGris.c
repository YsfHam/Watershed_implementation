/* ////////////////////////////////////////////////////////////////////////
* ////////////////////      Fichier : ImageGris.c       ///////////////////
* ////////////////////      Auteur : Youssef HAMDOUNI   ///////////////////
* ///////////////////////////////////////////////////////////////////////// */

#include "ImageGris.h"
#include "../lib/lodepng.h"
#include "Log.h"
#include "PairUIntMap_u8.h"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) > (b) ? (b) : (a))

////////////// Fonctions auxiliares //////////////////////////

static void TrouverMinMax(ImageGris *img, uint32_t rayon, uint32_t debutX, uint32_t debutY, uint8_t *min, uint8_t *max)
{
    *min = 255;
    *max = 0;
    for (int32_t y = MAX((int32_t)(debutY - rayon), 0); 
        y <= MIN((int32_t)(debutY + rayon), (int32_t)(img->height - 1)); 
        y++)
    {
        for (int32_t x = MAX((int32_t)(debutX - rayon), 0); 
        x <= MIN((int32_t)(debutX + rayon), (int32_t)(img->width - 1)); 
        x++)
        {
            if (img->colChannel[y * img->width + x] < *min)
                *min = img->colChannel[y * img->width + x];
            else if (img->colChannel[y * img->width + x] > *max)
                *max = img->colChannel[y * img->width + x];
        }
    }
}

////////////// Fonctions principales //////////////////////////

ImageGris* LireImage(const char *nom_fichier)
{
    ImageGris *img = calloc(1, sizeof(ImageGris));
    ASSERT(img, "LireImage : Erreur allocation mémoire\n");
    uint8_t *pixels;

    uint32_t erreur = lodepng_decode32_file(&pixels, &img->width, &img->height, nom_fichier);

    ASSERT_A(!erreur, "LireImage : Erreur lors du chargement de l'image %s\nCode Erreur %u : %s",
            nom_fichier, erreur, lodepng_error_text(erreur));

    img->colChannel = calloc(img->width * img->height, sizeof(uint8_t));
    ASSERT(img->colChannel, "LireImage : Erreur allocation mémoire\n");

    for (uint32_t y = 0; y < img->height; y++)
    {
        for (uint32_t x = 0; x < img->width; x++)
            img->colChannel[y * img->width + x] = pixels[y * img->width * 4 + x * 4];
    }

    free(pixels);

    return img;
}

void EcrireImage(ImageGris *img, const char *nom_fichier)
{
    uint8_t *pixels = calloc(img->width * img->height * 4, sizeof(uint8_t));

    for (uint32_t y = 0; y < img->height; y++)
    {
        for (uint32_t x = 0; x < img->width; x++)
        {
            pixels[y * img->width * 4 + x * 4 + 0] = img->colChannel[y * img->width + x];
            pixels[y * img->width * 4 + x * 4 + 1] = img->colChannel[y * img->width + x];
            pixels[y * img->width * 4 + x * 4 + 2] = img->colChannel[y * img->width + x];
            pixels[y * img->width * 4 + x * 4 + 3] = 255;
        }
    }

    uint32_t erreur = lodepng_encode32_file(nom_fichier, pixels, img->width, img->height);
    free(pixels);
    ASSERT_A(!erreur, "EcrireImage : Erreur lors de l'écriture de l'image %s\nCode Erreur %u : %s",
            nom_fichier, erreur, lodepng_error_text(erreur));
}

ImageGris* AllouerImage(uint32_t hauteur, uint32_t largeur)
{
    ImageGris *img = calloc(1, sizeof(ImageGris));
    ASSERT(img, "AllouerImage : Erreur allocation mémoire\n");
    img->colChannel = calloc(hauteur * largeur, sizeof(uint8_t));
    ASSERT(img->colChannel, "AllouerImage : Erreur allocation mémoire\n");
    img->height = hauteur;
    img->width = largeur;
    return img;
}

void LibererImage(ImageGris *imgG)
{
    ASSERT(imgG, "LibererImage : Erreur pointeur null !!!!!\n");
    free(imgG->colChannel);
    free(imgG);
}

ImageGris* CalculerGradient(ImageGris *img, uint32_t rayon)
{
    ImageGris *gradient = AllouerImage(img->height, img->width);
    uint8_t min, max;

    for (uint32_t y = 0; y < img->height; y++)
    {
        for (uint32_t x = 0; x < img->width; x++)
        {
            TrouverMinMax(img, rayon, x, y, &min, &max);
            gradient->colChannel[y * gradient->width + x] = max - min;
        }
    }

    return gradient;
}

ImageGris* CalculerLPE(ImageGris *gradient, ImageGris *marqueur)
{
    ImageGris *lpe = AllouerImage(marqueur->height, marqueur->width);

    PairUIntMap_u8 *L = map_new();
    uint32_t y, x;

    for (y = 0; y < marqueur->height; y++)
    {
        for (x = 0; x < marqueur->width; x++)
        {
            lpe->colChannel[y * lpe->width + x] = marqueur->colChannel[y * marqueur->width + x];
            if (marqueur->colChannel[y * marqueur->width + x])
            {
                map_inserer(L, gradient->colChannel[y * gradient->width + x],
                        pair_cons(x, y));

            }
                
        }
    }
    
    while (!map_isEmpty(L))
    {
        PairUint elem = map_retirer(L);

        
        // on ignore les pixel (x, y) tq x < 0 ou y < 0 ou x >= width ou x >= height
        for (y = (uint32_t)MAX((int32_t)(elem.y - 1), 0)
        ; y <= MIN(elem.y + 1, gradient->height - 1)
        ; y++)
        {
            for (x = (uint32_t)MAX((int32_t)(elem.x - 1), 0)
            ; x <= MIN(elem.x + 1, gradient->width - 1)
            ; x++)
            {
                if (!lpe->colChannel[y * lpe->width + x])
                {
                    lpe->colChannel[y * lpe->width + x] 
                    = lpe->colChannel[elem.y * lpe->width + elem.x];
                    map_inserer(L, gradient->colChannel[y * gradient->width + x],
                                pair_cons(x, y));
                }
            }
        }
    }

    map_free(L);

    return lpe;

}

void EcrireLPECouleur(ImageGris *lpe, const char *nom_fichier)
{
    uint8_t *pixels = calloc(lpe->width * lpe->height * 4, sizeof(uint8_t));

    uint8_t dansObjet[3] = {
        (rand() % 256) & 255,
        (rand() % 256) | 255,
        (rand() % 256) ^ 255
    };

    uint8_t horsObjet[3] = {
        (dansObjet[0] & dansObjet[1]) | dansObjet[2],
        dansObjet[0] ^ (dansObjet[1] & dansObjet[2]),
        (dansObjet[0] | dansObjet[1]) ^ dansObjet[2]
    };

    uint8_t *c;

    for (uint32_t y = 0; y < lpe->height; y++)
    {
        for (uint32_t x = 0; x < lpe->width; x++)
        {
            c = dansObjet;
            if (lpe->colChannel[y * lpe->width + x] == 100)
                c = horsObjet;
            pixels[y * lpe->width * 4 + x * 4 + 0] = c[0];
            pixels[y * lpe->width * 4 + x * 4 + 1] = c[1];
            pixels[y * lpe->width * 4 + x * 4 + 2] = c[2];
            pixels[y * lpe->width * 4 + x * 4 + 3] = 255;
        }
    }

    uint32_t erreur = lodepng_encode32_file(nom_fichier, pixels, lpe->width, lpe->height);
    free(pixels);
    ASSERT_A(!erreur, "EcrireImage : Erreur lors de l'écriture de l'image %s\nCode Erreur %u : %s",
            nom_fichier, erreur, lodepng_error_text(erreur));
}

void RossortirContour(ImageGris *lpe, const char *nom_fichier_in, const char *nom_fichier_out)
{
    uint8_t *img;
    uint32_t width, height;
    uint32_t erreur = lodepng_decode32_file(&img, &width, &height, nom_fichier_in);

    printf("width %u, lpe->width %u\n", width, lpe->width);
    printf("height %u, lpe->height %u\n", height, lpe->height);

    ASSERT_A(!erreur, "LireImage : Erreur lors du chargement de l'image %s\nCode Erreur %u : %s",
            nom_fichier_in, erreur, lodepng_error_text(erreur));

    uint32_t x, y;
    Liste *l = liste_new();
    for (y = 0; y < lpe->height; y++)
    {
        for (x = 0; x < lpe->width; x++)
        {
            if (lpe->colChannel[y * lpe->width + x] == 200)
            {
                liste_ajouterElement(l, pair_cons(x, y - 1));
                liste_ajouterElement(l, pair_cons(x, y + 1));
                liste_ajouterElement(l, pair_cons(x - 1, y));
                liste_ajouterElement(l, pair_cons(x - 1, y - 1));
                liste_ajouterElement(l, pair_cons(x - 1, y + 1));
                liste_ajouterElement(l, pair_cons(x + 1, y));
                liste_ajouterElement(l, pair_cons(x + 1, y - 1));
                liste_ajouterElement(l, pair_cons(x + 1, y + 1));
            }
        }
    }
    PairUint p;
    _Bool inBound;
    while (!liste_isEmpty(l))
    {
        p = liste_retirerElement(l);

        inBound = (p.x >= 0) && (p.x < width) &&
                  (p.y >= 0) && (p.y < height);
                
        if (inBound && lpe->colChannel[p.y * width + p.x] == 100)
        {
            img[p.y * width * 4 + p.x * 4] = 255;
            img[p.y * width * 4 + p.x * 4 + 1] = 0;
            img[p.y * width * 4 + p.x * 4 + 2] = 0;
        }
    }
    liste_free(l);
    erreur = lodepng_encode32_file(nom_fichier_out, img, width, height);
    free(img);
    ASSERT_A(!erreur, "EcrireImage : Erreur lors de l'écriture de l'image %s\nCode Erreur %u : %s",
            nom_fichier_out, erreur, lodepng_error_text(erreur));
}