#ifndef FLUX_H
#define FLUX_H

#include "global.h"

typedef struct s_flux
{
        int fid;
        char s[1024]; //
        char d[1024]; //

        int nb_paquet;
        int nb_paquet_emis;
        int nb_paquet_perdus;
        int nb_paquet_recus;
        float t_debut;
        float t_fin;

        struct s_flux *next;

} p_Flux,*Flux;

typedef struct s_listeflux
{
        Flux premier;

} p_listeFlux, *ListeFlux;

Flux newFlux(void);
Flux fluxExistant(Flux f,int fid);
void insertionFlux(ListeFlux f,GlobalData data,Trace t);
void affichage_donnees_flux(Flux f,int nb);


#endif
