#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/flux.h"



Flux newFlux(void){
        return (Flux) NULL;
}

Flux fluxExistant(Flux f,int fid)
{
        // Je cherche si le flux est deja dans ma liste ?
        Flux ptr = f;

        while(ptr != NULL)
        {
                if (ptr->fid == fid )
                        return ptr;  // Existant je renvoie son addresse


                ptr=ptr->next;
        }

        return NULL; // Je renvoie null pour dire qu'il faut creer le noeud

}

void insertionFlux(ListeFlux f,GlobalData data,Trace t){


        Flux ptr=newFlux();
        ptr = fluxExistant(f->premier,t->fid);

        if ( ptr == NULL )         // Je dois creer le flux
        {
                data->nb_flux++;
                ptr = (Flux) malloc(sizeof(p_Flux));
                if ( ptr != NULL)
                {
                        ptr->fid=t->fid;
                        strcpy(ptr->s,t->s);
                        strcpy(ptr->d,t->d);
                        ptr->nb_paquet=0;
                        ptr->nb_paquet_perdus=0;
                        ptr->nb_paquet_emis=0;
                        ptr->nb_paquet_recus=0;
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
        if (ptr->fid == t->fid )      // INUTILE ?
        {

                ptr->t_fin=t->t;

                switch (t->code)
                {
                case 0:
                        ptr->nb_paquet_emis++;
                        break;
                case 1:

                        break;
                case 2:

                        break;
                case 3:
                        ptr->nb_paquet_recus++;
                        break;
                case 4:
                        ptr->nb_paquet_perdus++;
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
        printf("%-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n", "Flux", "Source", "Dest", "Emis", "Recus","Perdus","Debut","Fin", "Duree de vie","Taux de perte");
        while(ptr != NULL )
        {
                if ( ptr->fid == nb)
                {
                        float taux = (float)ptr->nb_paquet_perdus/((float)ptr->nb_paquet_emis)*100;
                        printf(" %-10d %-10s %-10s %-10d %-10d %-10d %-10f %-10f %-10f %-10f%s \n",
                               ptr->fid,ptr->s,ptr->d,ptr->nb_paquet_emis,ptr->nb_paquet_recus,ptr->nb_paquet_perdus,
                               ptr->t_debut,ptr->t_fin,ptr->t_fin-ptr->t_debut,taux,"%");

                }
                ptr=ptr->next;
        }
        printf("-------------------------- \n\n");
}
