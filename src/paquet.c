#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/paquet.h"



Paquet newPaquet(void){
        return (Paquet) NULL;
}



Paquet getPaquet(Paquet p,int pid)
{
        // Je cherche si le paquet estdans ma liste ?
        Paquet ptr = p;

        while(ptr != NULL)
        {
                if (ptr->pid == pid )
                        return ptr;  // Existant je renvoie son addresse

                ptr=ptr->next;
        }

        return NULL; // Je renvoie null pour dire qu'il faut creer le paquet

}


void insertionPaquet(ListePaquet p,GlobalData data,Trace t){

        Paquet ptr=newPaquet();
        if ( t->code == 0 )         // Je dois creer le flux
        {
                ListeChemin lchemin = (ListeChemin ) malloc(sizeof(p_listechemin));
                ptr = (Paquet) malloc(sizeof(p_paquet));
                if ( ptr != NULL)
                {
                        ptr->pid=t->pid;
                        strcpy(ptr->s,t->s);
                        strcpy(ptr->d,t->d);
                        ptr->temps_attente_files = 0;
                        ptr->temps_transmission_liens=0;

                        Chemin c = (Chemin) malloc(sizeof(p_Chemin));
                        strcpy(c->pos,t->pos);
                        c->date=t->t;
                        c->code=t->code;
                        c->next = NULL;
                        lchemin->premier = c;
                        lchemin->dernier = c;

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
                ptr = getPaquet(p->premier,t->pid); // Cherche le paquet = t->pid dans la liste
                if ( ptr != NULL ) // Si je le trouve
                {
                        // J'ajoute un element dans la liste chemin
                        Chemin c = (Chemin) malloc(sizeof(p_Chemin));
                        strcpy(c->pos,t->pos);
                        c->date=t->t;
                        c->code=t->code;
                        c->next = NULL;

                        Chemin ptr2 = ptr->lc->premier;
                        while(ptr2->next != NULL) ptr2 = ptr2->next;  // Ajout en fin de liste
                        ptr2->next = c;

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
                                if ( ptr2->code == 2 && precedent->code == 0 )
                                        ptr->temps_attente_files+=ptr2->date-precedent->date;

                                if ( ptr2->code == 3 && precedent->code == 1 )
                                        ptr->temps_attente_files+=ptr2->date-precedent->date;

                                if ( ptr2->code == 1 && precedent->code == 2 )
                                        ptr->temps_transmission_liens+=ptr2->date-precedent->date;

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

                        //printf("(%s , %f , %d) -> ",ptr2->pos,ptr2->date,ptr2->code);

                }
                //printf("%s \n", ptr->d);
                printf("%-10s %-10s %-10s %-15s %-15s %-15s \n", "Source", "Dest", "Taille", "Attente file", "Temps transmission", "Délai acheminement");
                printf(" %-10s %-10s %-10d %-15f %-15f %-15f \n",ptr->s,ptr->d,ptr->taille,ptr->temps_attente_files,ptr->temps_transmission_liens,ptr->temps_attente_files+ptr->temps_transmission_liens);
                printf("---- \n");
                printf("-------------------------- \n\n");
        }
        else
        {
                printf("Paquet non trouvé ! \n");
        }
}
