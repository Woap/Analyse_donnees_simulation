#ifndef NOEUD_H
#define NOEUD_H

#include "global.h"

typedef struct s_noeud
{
        char nom[1024];

        int nb_paquet_emis;
        int nb_paquet_recus;
        int nb_paquet_entres;
        int nb_paquet_sortis;
        int nb_paquet_perdus;

        char nom_noeud_prec[1024];

        int nb_paquet_file;
        int taille_file;

        struct s_noeud *next;

} p_Noeud,*Noeud;


typedef struct s_listenoeud
{
        Noeud premier;

} p_listeNoeud, *ListeNoeud;


Noeud newNoeud(void);
Noeud noeudExistant(Noeud n,char nom[1024]);
void insertionNoeud(ListeNoeud n,GlobalData data,Trace t);
void affichage_donnees_noeud(Noeud n,GlobalData data);



#endif
