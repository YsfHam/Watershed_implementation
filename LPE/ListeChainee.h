/* ////////////////////////////////////////////////////////////////////////
* ////////////////////      Fichier : ListeChainee.h    ///////////////////
* ////////////////////      Auteur : Youssef HAMDOUNI   ///////////////////
* ///////////////////////////////////////////////////////////////////////// */

#ifndef _LISTE_CHAINEE_H_
#define _LISTE_CHAINEE_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/*
* Cette structure représente un pair d'entiers non signés sur 32 bits
* */
typedef struct PairUint
{
    uint32_t x;
    uint32_t y;
}PairUint;

/*
* Cette structure représente un noeud d'une liste chainée de PairUint
* pair : la valeur du noeud
* next : pointeur vers le noeud suivan dans la liste
* */
typedef struct Maillon
{
    PairUint pair;
    struct Maillon *next;
}Maillon;

/*
* Liste une structure qui contient un pointeur vers le premier noeud de la liste chainée
* On peut utiliser comme alternative (dans ce cas ) : typedef Maillon *Liste
* */
typedef struct Liste
{
    Maillon *tete;
}Liste;

/*
* pair_cons : constructeur de la structure PairUint
* @retour : renvoie une structure PairUint
* */
PairUint pair_cons(uint32_t x, uint32_t y);

/*
* maillon_new : construire un nouveau noeud
* p : la valeur du noeud
* @return : Un pointeur vers le nouveau noeud
* */
Maillon* maillon_new(PairUint p);

/*
* liste_new : construire une liste vide
* @return : renvoie un pointeur vers la liste
* */
Liste* liste_new();

/*
* liste_ajouterElement : ajoute l'élément p dans la liste l
* l : un pointeur vers la liste
* p : le nouveau élément
* @requis : l != NULL
* */
void liste_ajouterElement(Liste *l, PairUint p);

/*
* liste_retirerElement : retirer le premier élément de la liste
* l : un pointeur vers une liste
* @requis : l != NULL
* @retour : l'élément retiré
* */
PairUint liste_retirerElement(Liste *l);

/*
* liste_free : libère la zone mémoire occupée par la liste
* l : un pointeur vers la liste
* @requis l != NULL
* */
void liste_free(Liste *l);

/*
* liste_isEmpty : teste si la liste est vide ou pas
* l : un pointeur vers la liste
* @requis l != NULL
* @retour : - true si la liste est vide
*           - false sinon
* */
_Bool liste_isEmpty(Liste *l);

#endif