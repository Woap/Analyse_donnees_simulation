/*
 * \file noeud.h
 * \author IBIS Ibrahim
 * \date 3 novembre 2017
 *
 * Declarations des fonctions et structures
 *
 */

#ifndef NOEUD_H
#define NOEUD_H

#include "global.h"

typedef struct s_noeud
{
        char* nom;
        int nb_paquet_emis;
        int nb_paquet_recus;
        int nb_paquet_entres;
        int nb_paquet_sortis;
        int nb_paquet_perdus;

        int nb_paquet_file;
        int taille_file;

        struct s_noeud *next;

} p_Noeud,*Noeud;


typedef struct s_listenoeud
{
        Noeud premier;

} p_listeNoeud, *ListeNoeud;


Noeud newNoeud(void);
void freeNoeud(ListeNoeud ln);
Noeud noeudExistant(Noeud n,char* nom);
void insertionNoeud(ListeNoeud n,GlobalData data,Trace t,Param params);
void affichage_donnees_noeud(Noeud n,GlobalData data);
void affichage_donnees_noeud_client(Noeud n,GlobalData data,char nom[]);



#endif
