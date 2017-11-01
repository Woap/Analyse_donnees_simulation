#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/noeud.h"



Noeud newNoeud(void){
        return (Noeud) NULL;
}




Noeud noeudExistant(Noeud n,char nom[1024])
{
        // Je cherche si le noeud est deja dans ma liste ?
        Noeud ptr = n;

        while(ptr != NULL)
        {
                if (strcmp(ptr->nom, nom) == 0)
                        return ptr;  // Existant je renvoie son adresse

                ptr=ptr->next;
        }

        return NULL; // Je renvoie null pour dire qu'il faut creer le noeud

}

void insertionNoeud(ListeNoeud n,GlobalData data,Trace t){

        Noeud ptr=newNoeud();
        ptr = noeudExistant(n->premier,t->pos);

        if ( ptr == NULL ) // Je dois creer le noeud
        {
                data->nb_noeud++;
                ptr = (Noeud) malloc(sizeof(p_Noeud));
                if ( ptr != NULL)
                {
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
        if (strcmp(ptr->nom, t->pos) == 0) // INUTILE ?
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
                        ptr->nb_paquet_sortis++; // C'est le noeud d'avant CORRIGE
                        ptr->nb_paquet_file--;
                        break;
                case 3:
                        ptr->nb_paquet_recus++;
                        break;
                case 4:
                        ptr->nb_paquet_perdus++;
                        ptr->taille_file= ptr->nb_paquet_file;
                        break;
                default:

                        break;
                }
        }

        //return n;
}

void affichage_donnees_noeud(Noeud n,GlobalData data)
{
        Noeud ptr = n;
        printf("-------------------------- \n");
        printf("Statistiques des noeuds \n");
        printf("%-10s %-10s %-10s %-10s %-10s %-10s %-15s %-10s\n", "Noeud", "Emis", "Recus", "Entres","Sortis","Perdus","Taux de perte", "Proportion perte");
        while(ptr != NULL)
        {
                float proportion = (float)ptr->nb_paquet_perdus/(float)data->nb_paquet_perdus*100;
                float taux = (float)ptr->nb_paquet_perdus/((float)ptr->nb_paquet_emis+(float)ptr->nb_paquet_entres)*100;
                printf(" %-10s %-10d %-10d %-10d %-10d %-10d %-10f%-5s %-10f%s  \n",
                       ptr->nom,ptr->nb_paquet_emis,ptr->nb_paquet_recus,ptr->nb_paquet_entres,
                       ptr->nb_paquet_sortis,ptr->nb_paquet_perdus,taux,"%",proportion,"%");
                ptr=ptr->next;
        }
        printf("-------------------------- \n\n");
}
