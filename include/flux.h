/*
 * \file flux.h
 * \author IBIS Ibrahim
 * \date 3 novembre 2017
 *
 * Declarations des fonctions et structures
 *
 */

#ifndef FLUX_H
#define FLUX_H

#include "global.h"

typedef struct s_flux
{
        int fid;
        char* s; // noeud source
        char* d; // noeud destination

        int nb_paquet;
        int nb_paquet_emis;
        int nb_paquet_perdus;
        int nb_paquet_recus;
        int flux_etat; // 0 = inactif > 0 = nombre de paquet en transit
        float t_debut; // date debut
        float t_fin; // date fin

        float delai_moyen; 

        struct s_flux *next;

} p_Flux,*Flux;

typedef struct s_listeflux
{
        Flux premier;

} p_listeFlux, *ListeFlux;

Flux newFlux(void);
void freeFlux(ListeFlux lf);
Flux fluxExistant(Flux f,int fid);
void insertionFlux(ListeFlux f,GlobalData data,Trace t, Param params);
void affichage_donnees_flux(Flux f,int nb);


#endif
