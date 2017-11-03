/*
 * \file flux.c
 * \author IBIS Ibrahim
 * \date 3 novembre 2017
 *
 * Gestion des flux de la trace
 *
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/flux.h"

Flux newFlux(void){
        return (Flux) NULL;
}

void freeFlux(ListeFlux lf){
        Flux ptr = NULL;
        ptr = lf->premier;
        Flux curr= NULL;

        while (ptr != NULL) {
                curr = ptr;
                ptr = ptr->next;
                free(curr->s);
                free(curr->d);
                free (curr);
        }

        free(lf);
        printf("Nettoyage flux OK \n");
}

Flux fluxExistant(Flux f,int fid)
{
        // Je cherche si le flux est deja dans ma liste ?
        Flux ptr = NULL;
        ptr = f;

        while(ptr != NULL)
        {
                if (ptr->fid == fid )
                        return ptr;  // Existant je renvoie son addresse


                ptr=ptr->next;
        }

        return NULL; // Je renvoie null pour dire qu'il faut creer le noeud

}

void insertionFlux(ListeFlux f,GlobalData data,Trace t, Param params){


        Flux ptr=newFlux();
        ptr = fluxExistant(f->premier,t->fid);

        if ( ptr == NULL )         // Je dois creer le flux
        {
                data->nb_flux++;
                ptr = (Flux) malloc(sizeof(p_Flux));
                if ( ptr != NULL)
                {
                        ptr->fid=t->fid;
                        ptr->s = (char*)malloc(strlen(t->s)+1);
                        strcpy(ptr->s,t->s);
                        ptr->d = (char*)malloc(strlen(t->d)+1);
                        strcpy(ptr->d,t->d);
                        ptr->nb_paquet=0;
                        ptr->nb_paquet_perdus=0;
                        ptr->nb_paquet_emis=0;
                        ptr->nb_paquet_recus=0;
                        ptr->delai_moyen=0;
                        ptr->flux_etat=0; // 0 inactif > 0 nb paquet en transit
                        ptr->t_debut=t->t;
                        ptr->t_fin=t->t;

                        if ( f->premier == NULL )
                        {
                                f->premier = ptr;
                        }
                        else
                        {
                                ptr->next = f->premier;
                                f->premier = ptr;
                        }
                }
        }

        // Sinon ptr pointe vers le flux deja existant
        // traitement
        if (ptr->fid == t->fid )
        {

                ptr->t_fin=t->t;

                switch (t->code)
                {
                case 0:
                        ptr->nb_paquet_emis++;
                        if ( ptr->flux_etat == 0)
                        {
                                data->nb_flux_actif++;
                                if ( params->flux_actif )
                                        fprintf(params->filefluxactif, "%f %d \n", t->t, data->nb_flux_actif );
                        }
                        ptr->flux_etat++;
                        break;
                case 1:
                        if ( ptr->flux_etat == 0)
                        {
                                data->nb_flux_actif++;
                                if ( params->flux_actif )
                                        fprintf(params->filefluxactif, "%f %d \n", t->t, data->nb_flux_actif );
                        }
                        ptr->flux_etat++;
                        break;
                case 2:

                        break;
                case 3:

                        ptr->flux_etat--;

                        if ( ptr->flux_etat == 0)
                        {
                                data->nb_flux_actif--;
                                if ( params->flux_actif )
                                        fprintf(params->filefluxactif, "%f %d \n", t->t, data->nb_flux_actif );
                        }
                        break;
                case 4:
                        ptr->nb_paquet_perdus++;
                        ptr->flux_etat--;
                        if ( ptr->flux_etat == 0)
                        {
                                data->nb_flux_actif--;
                                if ( params->flux_actif )
                                        fprintf(params->filefluxactif, "%f %d \n", t->t, data->nb_flux_actif );
                        }
                        break;
                default:

                        break;
                }
        }

}

void affichage_donnees_flux(Flux f,int nb)
{
        Flux ptr = f;

        printf("-------------------------- \n");
        printf("Statistiques des flux \n");
        printf("%-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n", "Flux", "Source", "Dest", "Emis", "Recus","Perdus", "Delai moyen","Debut","Fin", "Duree de vie","Taux de perte");
        while(ptr != NULL && ptr->fid != nb )
        {
                ptr=ptr->next;
        }

        if ( ptr != NULL)
        {

                ptr->delai_moyen = (float)ptr->delai_moyen/(float)ptr->nb_paquet_recus;
                float taux = (float)ptr->nb_paquet_perdus/((float)ptr->nb_paquet_emis)*100;
                printf(" %-10d %-10s %-10s %-10d %-10d %-10d %-10f %-10f %-10f %-15f %-10f%s \n",
                       ptr->fid,ptr->s,ptr->d,ptr->nb_paquet_emis,ptr->nb_paquet_recus,ptr->nb_paquet_perdus,ptr->delai_moyen,
                       ptr->t_debut,ptr->t_fin,ptr->t_fin-ptr->t_debut,taux,"%");


        }
        else
        {
                printf("Flux non trouvé \n");
        }
        printf("-------------------------- \n\n");

}
