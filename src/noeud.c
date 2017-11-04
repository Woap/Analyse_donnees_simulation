/*
 * \file noeud.c
 * \author IBIS Ibrahim
 * \date 3 novembre 2017
 *
 * Gestion des noeuds de la trace
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/noeud.h"



Noeud newNoeud(void){
        return (Noeud) NULL;
}


void freeNoeud(ListeNoeud ln){
        Noeud ptr = NULL;
        ptr = ln->premier;
        Noeud curr= NULL;

        while (ptr != NULL) {
                curr = ptr;
                ptr = ptr->next;
                free(curr->nom);
                free (curr);
        }

        free(ln);

}




Noeud noeudExistant(Noeud n,char nom[1024])
{
        // Je cherche si le noeud est deja dans ma liste ?
        Noeud ptr = NULL;
        ptr = n;

        while(ptr != NULL)
        {
                if (strcmp(ptr->nom, nom) == 0)
                        return ptr;  // Existant je renvoie son adresse

                ptr=ptr->next;
        }

        return NULL; // Je renvoie null pour dire qu'il faut creer le noeud

}

void insertionNoeud(ListeNoeud n,GlobalData data,Trace t,Param params){

        Noeud ptr=newNoeud();
        ptr = noeudExistant(n->premier,t->pos);

        if ( ptr == NULL ) // Je dois creer le noeud
        {
                data->nb_noeud++;
                ptr = (Noeud) malloc(sizeof(p_Noeud));
                if ( ptr != NULL)
                {

                        ptr->nom = (char*)malloc(strlen(t->pos)+1);
                        strcpy(ptr->nom,t->pos);

                        ptr->nb_paquet_entres=0;
                        ptr->nb_paquet_sortis=0;
                        ptr->nb_paquet_perdus=0;
                        ptr->nb_paquet_emis=0;
                        ptr->nb_paquet_recus=0;
                        ptr->nb_paquet_file=0;
                        ptr->taille_file=0;

                        if ( n->premier == NULL )
                        {
                                n->premier = ptr;
                        }
                        else
                        {
                                ptr->next = n->premier;
                                n->premier = ptr;
                        }
                }
        }

        // Sinon ptr pointe vers le noeud deja existant
        // traitement
        if (strcmp(ptr->nom, t->pos) == 0)
        {

                switch (t->code)
                {
                case 0:
                        ptr->nb_paquet_emis++;
                        ptr->nb_paquet_file++;
                        break;
                case 1:
                        ptr->nb_paquet_entres++;
                        ptr->nb_paquet_file++;
                        break;
                case 2:
                        break;
                case 3:
                        ptr->nb_paquet_recus++;

                        ptr->nb_paquet_file--;
                        break;
                case 4:
                        ptr->nb_paquet_perdus++;
                        ptr->nb_paquet_file--;
                        if ( ptr->nb_paquet_perdus == 1  )
                        {
                                ptr->taille_file=ptr->nb_paquet_file;

                        }

                        break;
                default:

                        break;
                }

                if ( strcmp(params->noeud, ptr->nom) == 0 )
                {
                        if ( (t->code != 2) && (precision == 0) )
                        {
                                fprintf(params->fileatt, "%f %d \n", t->t, ptr->nb_paquet_file);
                        }
                        precision=(precision+1)%250;
                }


        }


}

void affichage_donnees_noeud(Noeud n,GlobalData data)
{
        Noeud ptr = NULL;
        ptr = n;
        printf("-------------------------- \n");
        printf("Statistiques des noeuds \n");

        printf("%-10s %-10s %-10s %-10s %-10s %-15s %-15s %-15s %-15s\n", "Noeud", "Emis", "Recus","Perdus", "| Taille file","Taux de perte", "Proportion perte", "Proportion emis", "Proportion reçus");
        while(ptr != NULL)
        {

                float proportion = (float)ptr->nb_paquet_perdus/(float)data->nb_paquet_perdus*100;
                float proportion_emis = (float)ptr->nb_paquet_emis/(float)data->nb_paquet_emis*100;
                float proportion_recus = (float)ptr->nb_paquet_recus/(float)data->nb_paquet_recus*100;
                float taux = (float)ptr->nb_paquet_perdus/((float)ptr->nb_paquet_emis+(float)ptr->nb_paquet_entres)*100;
                printf(" %-10s %-10d %-10d %-10d %-15d %f%-10s %f%-9s %f%-8s %-f%-7s  \n",
                       ptr->nom,ptr->nb_paquet_emis,ptr->nb_paquet_recus,ptr->nb_paquet_perdus,ptr->taille_file,taux,"%",proportion,"%",proportion_emis,"%",proportion_recus,"%");

                ptr=ptr->next;
        }
        printf("-------------------------- \n\n");
}

void affichage_donnees_noeud_client(Noeud n,GlobalData data,char nom[])
{
        Noeud ptr = n;
        printf("-------------------------- \n");
        printf("Statistiques du noeud \n");

        printf("%-10s %-10s %-10s %-10s %-10s %-15s %-15s %-15s %-15s\n", "Noeud", "Emis", "Recus","Perdus", "| Taille file","Taux de perte", "Proportion perte", "Proportion emis", "Proportion reçus");
        while(ptr != NULL && strcmp(ptr->nom,nom) != 0)
        {
                ptr=ptr->next;
        }

        if ( ptr !=NULL)
        {

                float proportion = (float)ptr->nb_paquet_perdus/(float)data->nb_paquet_perdus*100;
                float proportion_emis = (float)ptr->nb_paquet_emis/(float)data->nb_paquet_emis*100;
                float proportion_recus = (float)ptr->nb_paquet_recus/(float)data->nb_paquet_recus*100;
                float taux = (float)ptr->nb_paquet_perdus/((float)ptr->nb_paquet_emis+(float)ptr->nb_paquet_entres)*100;
                printf(" %-10s %-10d %-10d %-10d %-10d %-10f%s %f%-9s %f%-8s %f%-7s  \n",
                       ptr->nom,ptr->nb_paquet_emis,ptr->nb_paquet_recus,ptr->nb_paquet_perdus,ptr->taille_file,taux,"%",proportion,"%",proportion_emis,"%",proportion_recus,"%");
        }
        else
        {
                printf("Noeud non trouvé \n");
        }
        printf("-------------------------- \n\n");
}
