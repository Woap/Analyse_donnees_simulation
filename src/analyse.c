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
#include <unistd.h>
#include <math.h>



#include "../include/paquet.h"
#include "../include/flux.h"
#include "../include/noeud.h"
#include "../include/global.h"



void freeAll(ListeFlux liste_flux,GlobalData statistiques, ListePaquet liste_paquet, ListeNoeud liste_noeud, Param params)
{
        freeFlux(liste_flux);
        freeGlobal(statistiques);
        freePaquet(liste_paquet);
        freeNoeud(liste_noeud);
        free(params);
}


int lecture_trace(FILE *fp,ListeNoeud lnoeud,ListeFlux lflux,ListePaquet lpaquet,GlobalData data, int* nb, Param params)
{
        int trash;

        Trace t1 = NULL;
        t1 = (Trace) malloc(sizeof(p_trace));
        t1->code = -1;

        *nb = fscanf(fp, "%f %d",&t1->t,&t1->code);
        if ( t1->code != 4)
                *nb = fscanf(fp, "%d %d %d %d %s %s %s\n",&t1->pid,&t1->fid,&trash,&trash,t1->s,t1->d,t1->pos );
        else
                *nb = fscanf(fp, "%d %d %d %s %s %s\n",&t1->pid,&t1->fid,&trash,t1->s,t1->d,t1->pos );


        if ( *nb != EOF)
        {

                insertionNoeud(lnoeud,data,t1,params);
                insertionFlux(lflux,data,t1,params);
                insertionData(data,t1,params);
                insertionPaquet(lpaquet,lnoeud,lflux,data,t1,params);


        }

        free(t1);

        return 0;
}



void usage(char *prog)
{
        fprintf(stderr, "usage: %s [ -p <noeud> | all ] [ -t ] [ -l ] [ -f ] [-d <fid> ] <trace> <matrice_adjacence> \n  [ -p <noeud> ] : variation au cours du temps du nombre de paquets dans les files du <noeud> \n  [ -l ] : nombre de paquets perdus au cours du temps \n  [ -t ] : nombre de paquet en transit au cours du temps \n  [ -f ] : nombre de flux actif au cours du temps \n  [ -d <fid> ] : délai de bout en bout pour l'ensemble des paquets du flux <fid> au cours du temps \n", prog);
}


int main (int argc, char *argv[])
{

        FILE *file;
        int c;
        extern char * optarg;
        char * progname = argv[0];
        int errflg = 0;
        char c1[1]="";
        char chaine[10];
        int num=0;
        int retour=0;
        int nb=0;
        int i=0;
        int pflag=0;
        int dflag=0;
        int tflag=0;
        int fflag=0;
        int lflag=0;
        int decalage=0;


        if ( argc < 3  )
        {
                usage(argv[0]);
                return -1;
        }


        GlobalData statistiques = newData();

        ListeNoeud liste_noeud = NULL;
        liste_noeud  = (ListeNoeud) malloc(sizeof(p_listeNoeud));
        liste_noeud->premier=NULL;

        ListeFlux liste_flux = NULL;
        liste_flux = (ListeFlux) malloc(sizeof(p_listeFlux));
        liste_flux->premier=NULL;

        ListePaquet liste_paquet = NULL;
        liste_paquet = (ListePaquet) malloc(sizeof(p_listePaquet));
        liste_paquet->premier=NULL;

        Param params = NULL;
        params = (Param) malloc(sizeof(p_param));
        strcpy(params->noeud,"vide");
        params->flux_actif = 0;
        params->flux_delai =-1;
        params->transit=0;
        params->perdus=0;



        // Gestion argument

        while ((c = getopt(argc, argv, "p:tlfd:")) != -1)
                switch (c) {
                case 'p':
                        pflag++;
                        decalage+=2;
                        strcpy(params->noeud,optarg);
                        params->fileatt = fopen("fileatt.temp", "w");
                        precision = 0;
                        break;
                case 't':
                        tflag++;
                        decalage++;
                        params->transit++;
                        params->filetransit = fopen("transit.temp", "w");
                        precision2=0;
                        break;
                case 'f':
                        fflag++;
                        decalage++;
                        params->flux_actif++;
                        params->filefluxactif = fopen("flux_actif.temp", "w");
                        precision4=0;
                        break;
                case 'd':
                        dflag++;
                        decalage+=2;
                        params->flux_delai=atoi(optarg);
                        params->fileflux_delai = fopen("flux_delai.temp", "w");
                        precision1=0;
                        break;
                case 'l':
                        lflag++;
                        decalage++;
                        params->perdus++;
                        params->fileperdus = fopen("perdus.temp", "w");
                        break;
                case '?':
                        errflg++;
                        break;
                }

        if (errflg || argc != (decalage+3))
        {
                usage(progname);
                freeAll(liste_flux,statistiques,liste_paquet,liste_noeud,params);
                return -1;
        }

        file = fopen(argv[1+decalage], "r");

        if (!file)
        {
                fprintf(stderr, "Erreur ouverture du fichier %s \n",argv[1+decalage]);
                freeAll(liste_flux,statistiques,liste_paquet,liste_noeud,params);
                return -2;
        }

        int count = 0;
        char ca;

        for (ca = getc(file); ca != EOF; ca = getc(file))
                if (ca == '\n') // Increment count if this character is newline
                        count = count + 1;

        fclose(file);



        file = fopen(argv[1+decalage], "r");

        if (!file)
        {
                fprintf(stderr, "Erreur ouverture du fichier %s \n",argv[1+decalage]);
                freeAll(liste_flux,statistiques,liste_paquet,liste_noeud,params);
                return -2;
        }


        double progress=0;
        printf("Lecture trace \n");
        do {
                lecture_trace(file,liste_noeud,liste_flux,liste_paquet,statistiques,&nb,params);
                i++;
                progress = (double)i/(double)count*100;
                if ( ceil(progress) == progress )
                {
                        printf("\rProgression %f %s ",progress,"%");
                        fflush(stdout);
                }


        } while ( nb != EOF );

        printf("\n");

        affichage_donnees_globales(statistiques);
        affichage_donnees_noeud(liste_noeud->premier,statistiques);

        FILE * gnuplotPipe= NULL;
        FILE * gnuplotPipe1= NULL;
        FILE * gnuplotPipe2= NULL;
        FILE * gnuplotPipe3= NULL;
        FILE * gnuplotPipe4= NULL;




        // Gestion action

        while ( strcmp(c1,"4") != 0 )
        {

                printf(" Action possible : \n");
                printf(" 1. Analyser un paquet ( tapez 1 )\n");
                printf(" 2. Analyser un flux ( tapez 2 )\n");
                printf(" 3. Analyser un noeud ( tapez 3 )\n");
                printf(" 4. Quitter le programme ( tapez 4 )\n");
                printf("Entrée : ");
                retour = scanf ("%1s", c1);
                retour = scanf ("%*[^\n]");
                getchar ();


                if ( strcmp(c1,"1") == 0 )
                {
                        printf(" Tapez le numéro du paquet à analyser ( pid ) : ");
                        retour = scanf ("%d", &num);
                        retour = scanf ("%*[^\n]");
                        getchar ();

                        affichage_donnees_paquet(liste_paquet->premier,num);
                        num=0;

                }
                else if ( strcmp(c1,"2") == 0 )
                {
                        printf(" Tapez le numéro du flux à analyser ( fid ) : ");
                        retour = scanf ("%d", &num);
                        retour = scanf ("%*[^\n]");
                        getchar ();


                        affichage_donnees_flux(liste_flux->premier,num);
                        num=0;

                }
                else if ( strcmp(c1,"3") == 0 )
                {
                        printf(" Tapez le nom du noeud à analyser ( nom ) : ");
                        retour = scanf ("%10s", chaine);
                        retour = scanf ("%*[^\n]");
                        if ( retour != 0)
                                getchar ();


                        affichage_donnees_noeud_client(liste_noeud->premier,statistiques,chaine);

                }


        }

        // Gnuplot


        if ( pflag )
        {

                char titre [100];
                if ( strcmp(params->noeud,"all") != 0 )
                {
                        sprintf (titre, "set title \"Nombre de paquets en attente dans la file %s au cours du temps\"", params->noeud);

                }
                else
                {
                        sprintf (titre,"set title \"Nombre de paquets en attente dans l'ensemble des files au cours du temps\"" );
                }

                char * commandsForGnuplot[] = {titre,"set xlabel  'temps'", "plot 'fileatt.temp' with lines"};
                gnuplotPipe = popen ("gnuplot -persistent", "w");

                for (i=0; i < 3; i++)
                {
                        fprintf(gnuplotPipe, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
                }

                fclose(params->fileatt);
        }

        if ( dflag )
        {
                printf("\n");
                char titre2 [100];
                sprintf (titre2, "set title \"Délai de bout en bout pour les paquets du flux  %d au cours du temps\"", params->flux_delai);
                char * commandsForGnuplot[] = {titre2,"set xlabel  'temps'", "plot 'flux_delai.temp' with lines"};
                gnuplotPipe1 = popen ("gnuplot -persistent", "w");

                for (i=0; i < 3; i++)
                {
                        fprintf(gnuplotPipe1, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
                }
                fclose(params->fileflux_delai);


        }

        if ( tflag )
        {
                char * commandsForGnuplot[] = {"set title \"Nombre de paquets en transit au cours du temps\"","set xlabel  'temps'", "plot 'transit.temp' with lines"};
                gnuplotPipe2 = popen ("gnuplot -persistent", "w");

                for (i=0; i < 3; i++)
                {
                        fprintf(gnuplotPipe2, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
                }

                fclose(params->filetransit);
        }

        if ( fflag )
        {

                char * commandsForGnuplot[] = {"set title \"Nombre de flux actif au cours du temps\"","set xlabel  'temps'", "plot 'flux_actif.temp' with lines"};
                gnuplotPipe3 = popen ("gnuplot -persistent", "w");

                for (i=0; i < 3; i++)
                {
                        fprintf(gnuplotPipe3, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
                }

                fclose(params->filefluxactif);
        }

        if ( lflag )
        {

                char * commandsForGnuplot[] = {"set title \"Nombre de paquets perdus au cours du temps\"","set xlabel  'temps'", "plot 'perdus.temp' with lines"};
                gnuplotPipe4 = popen ("gnuplot -persistent", "w");

                for (i=0; i < 3; i++)
                {
                        fprintf(gnuplotPipe4, "%s \n", commandsForGnuplot[i]); //Send commands to gnuplot one by one.
                }

                fclose(params->fileperdus);


        }

        // FREE

        if ( gnuplotPipe != NULL )
                fclose(gnuplotPipe);
        if ( gnuplotPipe1  != NULL )
                fclose(gnuplotPipe1);
        if ( gnuplotPipe2  != NULL )
                fclose(gnuplotPipe2);
        if ( gnuplotPipe3  != NULL )
                fclose(gnuplotPipe3);
        if ( gnuplotPipe4  != NULL )
                fclose(gnuplotPipe4);

        freeAll(liste_flux,statistiques,liste_paquet,liste_noeud,params);



        fclose(file);

        return 0;
}
