/* ////////////////////////////////////////////////////////////////////////
* ////////////////////      Fichier : Log.h             ///////////////////
* ////////////////////      Auteur : Youssef HAMDOUNI   ///////////////////
*       Fichier regroupant des macros pour afficher les messages d'erreur
* ///////////////////////////////////////////////////////////////////////// */

#ifndef _LOG_H_
#define _LOG_H_
#include <stdio.h>
#include <assert.h>

#define ASSERT(cond, msg)\
if (!(cond)){ fprintf(stderr, msg); assert(0);}
#define ASSERT_A(cond, msg, ...)\
if (!(cond)){ fprintf(stderr, msg, __VA_ARGS__); assert(0);}

#define MEM_ALLOC_FAIL(pointer, funcName)\
ASSERT_A(pointer != NULL, "%s : Erreur d'allocation memoire\n", funcName);

#endif