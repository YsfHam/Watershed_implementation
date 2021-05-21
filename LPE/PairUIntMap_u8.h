/* ////////////////////////////////////////////////////////////////////////
* ////////////////////      Fichier : PairUintMap_u8.h  ///////////////////
* ////////////////////      Auteur : Youssef HAMDOUNI   ///////////////////
* ///////////////////////////////////////////////////////////////////////// */

#ifndef _PAIR_UINT_MAP_H_
#define _PAIR_UINT_MAP_H_

#include "ListeChainee.h"

/*
* Une table de hashage, sous la forme d'un tableau de listes chainées. chaque élément (v, c) est ajouté de la manière suivante
* v est ajouter à la liste dans la case c.
* */
typedef struct PairUIntMap_u8
{
    Liste *listsTab[256];
    uint16_t indiceMin;
}PairUIntMap_u8;

/*
* map_new : construire une nouvelle table de hashage
* @retour : un pointeur vers la nouvelle table de hashage
* */
PairUIntMap_u8* map_new();

/*
* map_inserer : insérer un nouvel (v, c) élément dans la table de hashage
* map : un pointeur vers la table de hashage
* key : la clé pour insérer l'élément dans la table de hashage
* value : la valeur ajoutée dans la table de hashage
* @requis : map != NULL
* */
void map_inserer(PairUIntMap_u8 *map, uint8_t key, PairUint value);

/*
* map_retirer : retire le premier élément dont la valeur de la clé est minimale
* map : un pointeur vers la table de hashage
* @requis : map != NULL
* @retour : la valeur de l'élément retirer
* */
PairUint map_retirer(PairUIntMap_u8 *map);

/*
* map_isEmpty : teste si la table de hashage est vide
* @requis : map != NULL
* @retour : - true si la table est vide
*           - false sinon
* */
_Bool map_isEmpty(PairUIntMap_u8 *map);

/*
* map_free : libère la mémoire occupée par la table de hashage
* @requis : map != NULL
* */
void map_free(PairUIntMap_u8 *map);


#endif