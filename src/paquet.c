/*
 * \file paquets.c
 * \author IBIS Ibrahim
 * \date 3 novembre 2017
 *
 * Gestion des paquets de la trace
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/paquet.h"



Paquet newPaquet(void){
        return (Paquet) NULL;
}

void freeChemin(ListeChemin lc){
        Chemin ptr = lc->premier;
        Chemin curr= NULL;

        while (ptr != NULL) { // set curr to head, stop if list empty.
                curr = ptr;
                ptr = ptr->next; // advance head to next element.
                free(curr->pos);
                free (curr);    // delete saved pointer.
        }

        free(lc);

}

void freePaquet(ListePaquet lp){
        Paquet ptr = lp->premier;
        Paquet curr= NULL;

        while (ptr != NULL) { // set curr to head, stop if list empty.
                curr = ptr;
                ptr = ptr->next; // advance head to next element.
                freeChemin(curr->lc);
                free(curr->s);
                free(curr->d);
                free(curr->prec);
                free (curr);    // delete saved pointer.
        }

        free(lp);

        printf("Nettoyage paquet OK \n");
}



Paquet getPaquet(Paquet p,int pid)
{
        Paquet ptr = p;

        while(ptr != NULL)
        {
                if (ptr->pid == pid )
                        return ptr;  // Existant je renvoie son adresse

                ptr=ptr->next;
        }

        return NULL; // Je renvoie null pour dire qu'il faut creer le paquet
}


void insertionPaquet(ListePaquet p,ListeNoeud n,ListeFlux f,GlobalData data,Trace t, Param params){

        Paquet ptr=newPaquet();
        if ( t->code == 0 )         // Je dois creer le paquet
        {
                ListeChemin lchemin = (ListeChemin ) malloc(sizeof(p_listechemin));
                ptr = (Paquet) malloc(sizeof(p_paquet));
                if ( ptr != NULL)
                {
                        ptr->pid=t->pid;

                        ptr->s = (char*)malloc(strlen(t->s)+1);
                        strcpy(ptr->s,t->s);
                        ptr->d = (char*)malloc(strlen(t->d)+1);
                        strcpy(ptr->d,t->d);
                        ptr->prec = (char*)malloc(strlen(t->pos)+1);
                        strcpy(ptr->prec,t->pos);

                        ptr->temps_attente_files = 0;
                        ptr->temps_transmission_liens=0;

                        Chemin c = (Chemin) malloc(sizeof(p_Chemin));
                        c->pos = (char*)malloc(strlen(t->pos)+1);
                        strcpy(c->pos,t->pos);
                        c->date=t->t;
                        c->code=t->code;
                        c->next = NULL;
                        lchemin->premier = c;
                        ptr->lc = lchemin;

                        if ( p->premier == NULL )
                        {
                                p->premier = ptr;
                        }
                        else
                        {
                                ptr->next = p->premier;
                                p->premier = ptr;
                        }
                }
        }
        else
        {
                ptr = getPaquet(p->premier,t->pid);   // Cherche le paquet
                if ( ptr != NULL )
                {
                        // J'ajoute un element dans la liste chemin

                        Chemin c = (Chemin) malloc(sizeof(p_Chemin)); // ACTUEL
                        c->pos = (char*)malloc(strlen(t->pos)+1);
                        strcpy(c->pos,t->pos);
                        c->date=t->t;
                        c->code=t->code;
                        c->next = NULL;

                        Chemin ptr2 = ptr->lc->premier;
                        while(ptr2->next != NULL) ptr2 = ptr2->next;  // Ajout en fin de liste
                        ptr2->next = c;

                        if ( c->code == 2 && ptr2->code == 0 )
                                ptr->temps_attente_files+=c->date-ptr2->date;

                        if ( c->code == 3 && ptr2->code == 1 )
                                ptr->temps_attente_files+=c->date-ptr2->date;

                        if ( c->code == 1 && ptr2->code == 2 )
                                ptr->temps_transmission_liens+=c->date-ptr2->date;


                        if ( t->code == 1 )
                        {

                                char *tmp = (char*)realloc(ptr->prec,strlen(t->pos)+1);
                                ptr->prec = tmp;
                                strcpy(ptr->prec,t->pos);
                        }

                        if ( t->code == 2 )
                        {
                                Noeud nptr = newNoeud();
                                nptr = noeudExistant(n->premier,ptr->prec);
                                if ( nptr != NULL)
                                {
                                        nptr->nb_paquet_sortis++;
                                        nptr->nb_paquet_file--;

                                        if ( strcmp(params->noeud, nptr->nom) == 0 )
                                        {
                                                if ( (precision == 0) )
                                                {
                                                        fprintf(params->fileatt, "%f %d \n", t->t, nptr->nb_paquet_file);
                                                }
                                                precision=(precision+1)%25;
                                        }
                                }
                        }

                        if ( t->code == 3 )
                        {

                                Flux fptr=newFlux();
                                data->temps_transmission_liens+= ptr->temps_transmission_liens;
                                data->temps_attente_files+=ptr->temps_attente_files;

                                fptr = fluxExistant(f->premier,t->fid);
                                fptr->delai_moyen+=ptr->temps_transmission_liens+ptr->temps_attente_files;
                                fptr->nb_paquet_recus++;
                                if ( params->flux_delai == t->fid )
                                {
                                        if ( (precision1 == 0) )
                                        {
                                                fprintf(params->fileflux_delai, "%f %f \n", t->t, (float)fptr->delai_moyen/(float)fptr->nb_paquet_recus);
                                        }
                                        precision1=(precision1+1)%25;
                                }
                        }
                }
        }
}


void affichage_donnees_paquet(Paquet p,int nb)
{
        Paquet ptr = p;

        printf("-------------------------- \n");
        printf("Statistiques des paquets \n");
        while(ptr != NULL && ptr->pid != nb )
        {
                ptr=ptr->next;
        }

        if ( ptr != NULL)
        {
                printf("---- Paquet %d ---- \n",ptr->pid);
                printf("Chemin emprunté \n ");
                Chemin ptr2 = ptr->lc->premier;
                printf("- Emission par %s ", ptr->s);

                Chemin precedent = NULL;
                Chemin precedent2 =NULL;
                while(ptr2 != NULL)
                {
                        precedent = ptr2;
                        ptr2 = ptr2->next;

                        if ( precedent != NULL && ptr2 != NULL)
                        {

                                if ( precedent->code == 0 )
                                        printf("( t = %f  ) \n ",precedent->date);
                                switch (ptr2->code)
                                {

                                case 1:
                                        printf("- Arrivée sur %s ( temps transmission %s -> %s = %f ) \n ",ptr2->pos,precedent2->pos,ptr2->pos,ptr2->date-precedent->date );
                                        break;
                                case 2:
                                        printf("- Depart depuis la file de %s ( temps attente = %f ) \n ",precedent->pos,ptr2->date-precedent->date );
                                        precedent2=precedent;
                                        break;
                                case 3:
                                        printf("- Arrivée à destination %s ( t = %f ( temps attente = %f) ) \n ",ptr2->pos,ptr2->date,ptr2->date-precedent->date );
                                        break;
                                case 4:
                                        printf("Perte du paquet sur %s \n",ptr2->pos);
                                        break;
                                default:

                                        break;
                                }
                        }
                }

                printf("%-10s %-10s %-15s %-15s %-20s \n", "Source", "Dest", "Attente file", "Temps transmission", "Délai acheminement");
                printf(" %-10s %-10s %-15f %-20f %-20f \n",ptr->s,ptr->d,ptr->temps_attente_files,ptr->temps_transmission_liens,ptr->temps_attente_files+ptr->temps_transmission_liens);
                printf("---- \n");

        }
        else
        {
                printf("Paquet non trouvé ! \n");
        }
        printf("-------------------------- \n\n");
}
