/* ////////////////////////////////////////////////////////////////////////
* ////////////////////      Fichier : ImageGris.h       ///////////////////
* ////////////////////      Auteur : Youssef HAMDOUNI   ///////////////////
* ///////////////////////////////////////////////////////////////////////// */

#ifndef _IMAGE_GRIS_H_
#define _IMAGE_GRIS_H_

#include <stdint.h>
#include <stdlib.h>

/*
* Cette structure représente une image gris donc la quantitée des couleurs RVBA est la même, sauf la valeur du transparence est toujours 255 (la valeur max)
* width : la longeur de l'image
* height : la largeur de l'image
* colChannel : un tableau contenant la quantité du gris pour chaque pixel de l'image
* */
typedef struct ImageGris
{
    uint32_t width;
    uint32_t height;
    uint8_t *colChannel;
}ImageGris;

/*
* LireImage : lit le contenu d'une image grise dont son nom est donné en paramètre
* nom_fichier : une chaîne de caractère. Le nom de l'image à lire
* @requis : nom_fichier != NULL
* @retour : un pointeur vers la structure ImageGris
* */
ImageGris* LireImage(const char *nom_fichier);

/*
* EcrireImage : enregistre une image "grise" à partir des donné de la structure ImageGris
* img : un pointeur vers la structure ImageGris
* nom_fichier : le chemin où l'image sera sauvegardée
* @requis : img != NULL
* @requis : nom_fichier != NULL
* */
void EcrireImage(ImageGris *img, const char *nom_fichier);

/*
* AllouerImage : construire une image vide de taille hauteur * largeur
* hauteur : la hauteur de l'image
* largeur : la largeur de l'image
* @retour : un pointeur vers la structure ImageGris
* */
ImageGris* AllouerImage(uint32_t hauteur, uint32_t largeur);

/*
* LibererImage : libère la mémoire allouée
* imgG : le pointeur vers la zone mémoire à libèrer
* @requis : imgG != NULL
* */
void LibererImage(ImageGris *imgG);

/*
* CalculerGradient : calcule l'image du gradient d'une image. Chaque pixel de la nouvelle image est obtnu en faisant la différence entre le max et le min des valeurs de gris des pixels voisins dans certain rayon r
* img : un pointeur vers ImageGris
* rayon : pour determiner les voisins de chaque pixel de la nouvelle image
* @requis : img != NULL
* @requis : rayon > 0
* @retour : l'image du gradient. Un pointeur vers la structure ImageGris
* */
ImageGris* CalculerGradient(ImageGris *img, uint32_t rayon);

/*
* CalculerLPE : calcule une nouvelle image en utilisant l'algorthme de la ligne de partage des eaux qui determine les contours d'un objet dans une image, en utilisant l'image du gradient et des marqueur
* gradient : Image du gradient
* marqueur : l'image des marqueur
* @requis : gradient != NULL
* @requis : marqeur != NULL
* @retour : l'image de la ligne de partage des eaux
* */
ImageGris* CalculerLPE(ImageGris *gradient, ImageGris *marqueur);

/*
* EcrireLPECouleur : cette fonction enregistre l'image obtenu par l'algorithme de la ligne de partage des eaux avec des couleurs
* lpe : pointeur vers ImageGris, c'est l'image obtenu par l'algo LPE
* nom_fichier : le nom du fichier où l'image sera enregistrée
* @requis : lpe != NULL
* @requis : nom_fichier != NULL
* */
void EcrireLPECouleur(ImageGris *lpe, const char *nom_fichier);

void RossortirContour(ImageGris *lpe, const char *nom_fichier_in, const char *nom_fichier_out);
#endif