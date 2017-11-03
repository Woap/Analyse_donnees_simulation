/*
 * \file paquet.h
 * \author IBIS Ibrahim
 * \date 3 novembre 2017
 *
 * Declarations des fonctions et structures
 *
 */

#ifndef PAQUET_H
#define PAQUET_H

#include "noeud.h"
#include "flux.h"


typedef struct s_chemin
{
        char* pos;
        float date;
        int code;

        struct s_chemin *next;

} p_Chemin,*Chemin;

typedef struct s_listechemin
{
        Chemin premier;

} p_listechemin, *ListeChemin;

typedef struct s_paquet
{
        int pid;

        char* s; // noeud source du paquet
        char* d; // noeud destination du paquet
        ListeChemin lc;

        char* prec;

        float temps_attente_files;
        float temps_transmission_liens;

        struct s_paquet *next;

} p_paquet,*Paquet;

typedef struct s_listepaquet
{
        Paquet premier;

} p_listePaquet, *ListePaquet;

Paquet newPaquet(void);
void freePaquet(ListePaquet lp);
Paquet getPaquet(Paquet p,int pid);
void insertionPaquet(ListePaquet p,ListeNoeud n,ListeFlux f,GlobalData data,Trace t, Param params);
void affichage_donnees_paquet(Paquet p,int nb);

#endif
