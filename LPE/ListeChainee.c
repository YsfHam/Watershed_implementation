/* ////////////////////////////////////////////////////////////////////////
* ////////////////////      Fichier : ListeChainee.c    ///////////////////
* ////////////////////      Auteur : Youssef HAMDOUNI   ///////////////////
* ///////////////////////////////////////////////////////////////////////// */

#include "ListeChainee.h"
#include "Log.h"

PairUint pair_cons(uint32_t x, uint32_t y)
{
    PairUint p;
    p.x = x;
    p.y = y;

    return p;
}

Maillon* maillon_new(PairUint p)
{
    Maillon *res = calloc(1, sizeof(Maillon));
    MEM_ALLOC_FAIL(res, "maillon_new");
    res->pair = p;
    res->next = NULL;

    return res;
}

Liste* liste_new()
{
    Liste *l = calloc(1, sizeof(Liste));
    MEM_ALLOC_FAIL(l, "liste_new");

    return l;
}

void liste_ajouterElement(Liste *l, PairUint p)
{
    if (l->tete)
    {
        Maillon *m = maillon_new(p);
        m->next = l->tete;
        l->tete = m;
    }
    else
        l->tete = maillon_new(p);
}

PairUint liste_retirerElement(Liste *l)
{
    Maillon *temp = l->tete;
    PairUint res = temp->pair;

    l->tete = temp->next;
    free(temp);

    return res;
}

void liste_free(Liste *l)
{
    while (!liste_isEmpty(l))
        liste_retirerElement(l);
    free(l);
}

_Bool liste_isEmpty(Liste *l)
{
    return !l->tete;
}