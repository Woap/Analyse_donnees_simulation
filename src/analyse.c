/*
 * \file analyse.c
 * \author IBIS Ibrahim
 * \date 11 octobre 2017
 *
 * Analyse de données de simulation
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "../include/paquet.h"
#include "../include/flux.h"
#include "../include/noeud.h"
#include "../include/global.h"

#define TAILLE 1024 /* read 1024 bytes at a time */


int lecture_trace(FILE *fp, Trace t,ListeNoeud lnoeud,ListeFlux lflux,ListePaquet lpaquet,GlobalData data, int* nb)
{
        int trash;

        Trace t1 = (Trace) malloc(sizeof(p_trace));

        *nb = fscanf(fp, "%f %d",&t1->t,&t1->code);
        if ( t1->code != 4)
                *nb = fscanf(fp, "%d %d %d %d %s %s %s\n",&t1->pid,&t1->fid,&trash,&trash,t1->s,t1->d,t1->pos );
        else
                *nb = fscanf(fp, "%d %d %d %s %s %s\n",&t1->pid,&t1->fid,&trash,t1->s,t1->d,t1->pos );

        if ( *nb != EOF)
        {
                t = t1;

                insertionNoeud(lnoeud,data,t);
                insertionFlux(lflux,data,t);
                insertionPaquet(lpaquet,data,t);
                insertionData(data,t);
        }

        free(t);

        return 0;
}


int main (int argc, char *argv[])
{

        FILE *file;
        if ( argc != 3 )
        {
                fprintf(stderr, "Usage : %s trace matrice_adjacence \n",argv[0]);
                return -1;
        }

        Trace trace = newTrace();
        GlobalData statistiques = newData();

        ListeNoeud liste_noeud = (ListeNoeud) malloc(sizeof(p_listeNoeud));
        liste_noeud->premier=NULL;

        ListeFlux liste_flux = (ListeFlux) malloc(sizeof(p_listeFlux));
        liste_flux->premier=NULL;

        ListePaquet liste_paquet = (ListePaquet) malloc(sizeof(p_listePaquet));
        liste_paquet->premier=NULL;

        int nb = 0;

        file = fopen(argv[1], "r");
        if (file) {
                do {
                        lecture_trace(file,trace,liste_noeud,liste_flux,liste_paquet,statistiques,&nb);

                } while ( nb != EOF );

        }

        affichage_donnees_globales(statistiques);
        affichage_donnees_noeud(liste_noeud->premier,statistiques);
        affichage_donnees_flux(liste_flux->premier,2);
        affichage_donnees_paquet(liste_paquet->premier,3018);

        if (ferror(file))
        {
                fprintf(stderr, "Erreur ouverture du fichier\n");
        }

        fclose(file);

        return 0;
}
