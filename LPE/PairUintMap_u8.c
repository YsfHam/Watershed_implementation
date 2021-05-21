/* ////////////////////////////////////////////////////////////////////////
* ////////////////////      Fichier : PairUintMap_u8.c  ///////////////////
* ////////////////////      Auteur : Youssef HAMDOUNI   ///////////////////
* ///////////////////////////////////////////////////////////////////////// */

#include "PairUIntMap_u8.h"
#include "Log.h"

PairUIntMap_u8* map_new()
{
    PairUIntMap_u8 *map = calloc(1, sizeof(PairUIntMap_u8));
    MEM_ALLOC_FAIL(map, "map_new");
    map->indiceMin = 256;

    return map;
}

void map_inserer(PairUIntMap_u8 *map, uint8_t key, PairUint value)
{
    if (!map->listsTab[key])
        map->listsTab[key] = liste_new();

    liste_ajouterElement(map->listsTab[key], value);

    if ((uint16_t)key < map->indiceMin)
        map->indiceMin = (uint16_t)key;
    
}

PairUint map_retirer(PairUIntMap_u8 *map)
{
    Liste *l = map->listsTab[map->indiceMin];
    PairUint res = liste_retirerElement(l);

    while ((!l || (l && liste_isEmpty(l))) && (map->indiceMin < 256))
    {
        map->indiceMin++;
        if (map->indiceMin < 256)
            l = map->listsTab[map->indiceMin];
    }

    return res;
}

_Bool map_isEmpty(PairUIntMap_u8 *map)
{
    return map->indiceMin == 256;
}

void map_free(PairUIntMap_u8 *map)
{
    for (uint16_t i = 0; i < 256; i++)
    {
        if (map->listsTab[i])
            liste_free(map->listsTab[i]);
    }

    free(map);
}