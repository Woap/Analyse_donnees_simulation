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

#include "listeDoubles.c"

#define TAILLE 1024 /* read 1024 bytes at a time */

char buf[TAILLE];
FILE *file;
size_t nread;

typedef struct s_trace {
        float t; // heure de l'evenement
        int code; // type de l'evenement
        int pid; // identifiant du paquet
        int fid; // identifidant du flux auquel appartient le paquet
        char s[1024]; // noeud source du paquet
        char d[1024]; // noeud destination du paquet
        char pos[1024];

        struct ld *next;

} p_trace, *Trace;

typedef struct s_flux
{
        int fid;
        char s[1024]; //
        char d[1024]; //

        int nb_paquet;
        int nb_paquet_emis;
        int nb_paquet_perdus;
        int nb_paquet_recus;
        float t_debut;
        float t_fin;

        struct s_flux *next;

} p_Flux,*Flux;

typedef struct s_listeflux
{
        Flux premier;

} p_listeFlux, *ListeFlux;

typedef struct s_noeud
{
        char nom[1024];

        int nb_paquet_emis;
        int nb_paquet_recus;
        int nb_paquet_entres;
        int nb_paquet_sortis;
        int nb_paquet_perdus;

        struct s_noeud *next;

} p_Noeud,*Noeud;

typedef struct s_listenoeud
{
        Noeud premier;

} p_listeNoeud, *ListeNoeud;

typedef struct s_paquet
{
        int pid;

        char s[1024]; // noeud source du paquet
        char d[1024]; // noeud destination du paquet
        int taille;

        struct s_paquet *next;

} p_paquet,*Paquet;

typedef struct s_listepaquet
{
        Paquet premier;

} p_listePaquet, *ListePaquet;


typedef struct s_globaldata
{
        int nb_paquet;
        int nb_paquet_traites;
        int nb_paquet_emis;
        int nb_paquet_recus;
        int nb_flux;
        int nb_paquet_perdus;
        int nb_evenement;
        int nb_noeud;
        float taux_perte;
        float* proportion_paquet_perdus;
        float delai_moyen;
        float* temps_attente_files;
        float* temps_transmission_liens;
        int* taille_files_attente;



} sglobalData, *GlobalData;




GlobalData newData(void){
        GlobalData nouveau = (GlobalData) malloc(sizeof(sglobalData));
        nouveau->nb_paquet_perdus=0;
        nouveau->nb_paquet_traites=0;
        nouveau->nb_paquet_emis=0;
        nouveau->nb_paquet_recus=0;
        nouveau->nb_flux=0;
        nouveau->nb_paquet_perdus=0;
        nouveau->taux_perte=0;
        nouveau->nb_noeud=0;

        nouveau->nb_paquet=0;
        //nouveau->proportion_paquet_perdus=0;
        nouveau->delai_moyen=0;
        nouveau->nb_evenement=0;

        //nouveau->temps_attente_files=0;
        //nouveau->temps_transmission_liens=0;
        //nouveau->taille_files_attente=0;

        return nouveau;


}

Trace newTrace(void){
        return (Trace) NULL;
}

Noeud newNoeud(void){
        return (Noeud) NULL;
}

Flux newFlux(void){
        return (Flux) NULL;
}

Paquet newPaquet(void){
        return (Paquet) NULL;
}


Noeud noeudExistant(Noeud n,char nom[1024])
{
        // Je cherche si le noeud est deja dans ma liste ?
        Noeud ptr = n;

        while(ptr != NULL)
        {
                if (strcmp(ptr->nom, nom) == 0)
                        return ptr;  // Existant je renvoie son addresse


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
                        break;
                case 1:
                        ptr->nb_paquet_entres++;
                        break;
                case 2:
                        ptr->nb_paquet_sortis++; // C'est le noeud d'avant CORRIGE
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

        //return n;
}

void insertionData(GlobalData data,Trace t){
        data->nb_evenement++;
        switch (t->code)
        {
        case 0:
                data->nb_paquet_emis++;
                break;
        case 1:

                break;
        case 2:
                data->nb_paquet_traites++;
                break;
        case 3:
                data->nb_paquet_recus++;
                break;
        case 4:
                data->nb_paquet_perdus++;
                break;
        default:

                break;
        }

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


void insertionPaquet(ListePaquet p,GlobalData data,Trace t){

        Paquet ptr=newPaquet();
        if ( t->code == 0 )         // Je dois creer le flux
        {
                data->nb_paquet++;
                ptr = (Paquet) malloc(sizeof(p_paquet));
                if ( ptr != NULL)
                {
                        ptr->pid=t->pid;
                        strcpy(ptr->s,t->s);
                        strcpy(ptr->d,t->d);

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


}




int lecture_trace(FILE *fp, Trace t,ListeNoeud lnoeud,ListeFlux lflux,ListePaquet lpaquet,GlobalData data, int* nb)
{
        int trash;

        Trace t1 = (Trace) malloc(sizeof(p_trace));

        *nb = fscanf(file, "%f %d",&t1->t,&t1->code);
        if ( t1->code != 4)
                *nb = fscanf(file, "%d %d %d %d %s %s %s\n",&t1->pid,&t1->fid,&trash,&trash,t1->s,t1->d,t1->pos );
        else
                *nb = fscanf(file, "%d %d %d %s %s %s\n",&t1->pid,&t1->fid,&trash,t1->s,t1->d,t1->pos );

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


void affichage_trace(listeDoubles g, int i)
{
        /*printf("Heure : %f \n",t->t); // heure de l'evenement
           printf("Type : %d \n",t->code); // type de l'evenement
           printf("ID PAQUET : %d \n",t->pid); // identifiant du paquet
           printf("ID FLUX : %d \n",t->fid); // identifidant du flux auquel appartient le paquet
           printf("NOEUD SOURCE : %s \n",t->s); // noeud source du paquet
           printf("NOEUD DEST : %s \n",t->d); // noeud destination du paquet
           printf("POS : %s \n\n\n",t->pos); // noeud destination du paquet*/

        printf("%d : %f , %d , %d , %d , %s , %s , %s \n ",i,g->t,g->code,g->pid,g->fid,
               g->s,g->d,g->pos);
}

void affichage_donnees_globales(GlobalData g)
{
        printf("Nombre evenement : %d \n",g->nb_evenement);
        printf("Nombre paquets traités : %d \n",g->nb_paquet_traites);
        printf("Nombre paquets émis : %d \n",g->nb_paquet_emis);
        printf("Nombre paquets reçus : %d \n",g->nb_paquet_recus);
        printf("Nombre de flux : %d \n",g->nb_flux);
        printf("Nombre de noeud : %d \n",g->nb_noeud);
        printf("Nombre de paquets perdus : %d \n",g->nb_paquet_perdus);
        g->taux_perte = (float)g->nb_paquet_perdus/(float)g->nb_paquet_emis*100;
        printf("Taux de perte : %f %% \n",g->taux_perte);
        // TABLEAU


        printf("Delai Moyen : %f \n",g->delai_moyen);

        printf("Nb paquet : %d \n",g->nb_paquet);

        // TABLEAU
        //printf("Heure : %f \n",temps_attente_files);

        // TABLEAU
        //printf("Heure : %f \n",temps_transmission_liens);

        // TABLEAU
        //printf("Heure : %f \n",taille_files_attente);

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
        printf("-------------------------- \n");
}

void affichage_donnees_flux(Flux f,int nb)
{
        Flux ptr = f;

        printf("-------------------------- \n");
        printf("Statistiques des flux \n");
        printf("%-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s %-10s\n", "Flux", "Source", "Dest", "Emis", "Recus","Perdus","Debut","Fin","Taux de perte");
        while(ptr != NULL )
        {
                if ( ptr->fid == nb)
                {
                        float taux = (float)ptr->nb_paquet_perdus/((float)ptr->nb_paquet_emis)*100;
                        printf(" %-10d %-10s %-10s %-10d %-10d %-10d %-10f %-10f %-10f%s \n",
                               ptr->fid,ptr->s,ptr->d,ptr->nb_paquet_emis,ptr->nb_paquet_recus,ptr->nb_paquet_perdus,
                               ptr->t_debut,ptr->t_fin,taux,"%");

                }
                ptr=ptr->next;
        }
        printf("-------------------------- \n");
}

void affichage_donnees_paquet(Paquet p,int nb)
{
        Paquet ptr = p;


        printf("-------------------------- \n");
        printf("Statistiques des paquets \n");
        printf("%-10s %-10s %-10s %-10s \n", "Paquet", "Source", "Dest", "Taille");
        while(ptr != NULL )
        {
                if ( ptr->pid == nb)
                {
                        printf(" %-10d %-10s %-10s %-10d \n",ptr->pid,ptr->s,ptr->d,ptr->taille);
                }
                ptr=ptr->next;
        }
        printf("-------------------------- \n");
}


int main (int argc, char *argv[])
{
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
        //affichage_donnees_flux(liste_flux->premier,1742);
        //affichage_donnees_paquet(liste_paquet->premier,519581);




        if (ferror(file))
        {
                fprintf(stderr, "Erreur ouverture du fichier\n");
        }

        fclose(file);

        return 0;
}
