#ifndef PAQUET_H
#define PAQUET_H

#include "global.h"


typedef struct s_chemin
{
        char pos[1024];
        float date;
        int code;

        struct s_chemin *next;

} p_Chemin,*Chemin;

typedef struct s_listechemin
{
        Chemin premier;
        Chemin dernier;

} p_listechemin, *ListeChemin;

typedef struct s_paquet
{
        int pid;

        char s[1024]; // noeud source du paquet
        char d[1024]; // noeud destination du paquet
        int taille;
        float t_depart;
        float t_arrive;
        ListeChemin lc;

        float temps_attente_files;
        float temps_transmission_liens;

        struct s_paquet *next;

} p_paquet,*Paquet;

typedef struct s_listepaquet
{
        Paquet premier;

} p_listePaquet, *ListePaquet;

Paquet newPaquet(void);
Paquet getPaquet(Paquet p,int pid);
void insertionPaquet(ListePaquet p,GlobalData data,Trace t);
void affichage_donnees_paquet(Paquet p,int nb);

#endif
