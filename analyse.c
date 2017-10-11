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
 
#define TAILLE 1024 /* read 1024 bytes at a time */

char buf[TAILLE];
FILE *file;
size_t nread;

typedef struct s_trace
{
    float t; // heure de l'evenement
    int code; // type de l'evenement
    int pid; // identifiant du paquet
    int fid; // identifidant du flux auquel appartient le paquet
    char s[1024] ; // noeud source du paquet
    char d[1024]; // noeud destination du paquet
    int pos;
} *p_Trace,Trace;


typedef struct s_globaldata
{
    int nb_paquet_traites;
    int nb_paquet_emis;
    int nb_paquet_recus;
    int nb_flux;
    int nb_paquet_perdus;
    float taux_perte;
    float* proportion_paquet_perdus;
    float delai_moyen;
    float* temps_attente_files;
    float* temps_transmission_liens;
    int* taille_files_attente;
    
} *p_GlobalData,GlobalData;


int lecture_trace(FILE *fp, Trace *t)
{
    int trash;
    if ( fscanf(file, "%f %d %d %d %d %d %s %s",&t->t,&t->code,&t->pid,&t->fid,&trash,&trash,t->s,t->d ) != 8)
        return -1;
          
    return 0;
}


void affichage_trace(Trace *t)
{
    printf("Heure : %f \n",t->t); // heure de l'evenement
    printf("Type : %d \n",t->code); // type de l'evenement
    printf("ID PAQUET : %d \n",t->pid); // identifiant du paquet
    printf("ID FLUX : %d \n",t->fid); // identifidant du flux auquel appartient le paquet
    printf("NOEUD SOURCE : %s \n",t->s); // noeud source du paquet
    printf("NOEUD DEST : %s \n\n\n",t->d); // noeud destination du paquet
    

}

void affichage_donnees_globales(GlobalData *g)
{
    printf("Nombre paquets traités : %d \n",g->nb_paquet_traites);
    printf("Nombre paquets émis : %d \n",g->nb_paquet_emis);
    printf("Nombre paquets reçus : %d \n",g->nb_paquet_recus);
    printf("Nombre de flux : %d \n",g->nb_flux);
    printf("Nombre de paquets perdus : %d \n",g->nb_paquet_perdus);
    printf("Taux de perte : %f \n",g->taux_perte);
    // TABLEAU
        //printf("Heure : %f \n",proportion_paquet_perdus);
        
    printf("DELAI MOYEN : %f \n",g->delai_moyen);
    
    // TABLEAU
        //printf("Heure : %f \n",temps_attente_files);
    
    // TABLEAU
        //printf("Heure : %f \n",temps_transmission_liens);
    
    // TABLEAU
        //printf("Heure : %f \n",taille_files_attente);

}
 
 
 
int main (int argc, char *argv[])
{
    if ( argc != 3 ) 
    {
        fprintf(stderr, "Usage : %s trace matrice_adjacence \n",argv[0]);  
        return -1;
    }
    
    int nb_ligne = 0;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;
    Trace* trace;
    
    trace = (Trace*)malloc(sizeof(Trace));
    
    
    file = fopen(argv[1], "r");
    if (file) {
        
        // TEST
        int trash;
        int i = 0;
        for ( i = 0 ; i < 10 ; i++ )
        {
            if ( lecture_trace(file,trace) == 0 )
            {
                affichage_trace(trace);
            }
        }
          
    }
        
    if (ferror(file)) 
    {
        fprintf(stderr, "Erreur ouverture du fichier\n");  
    }
    
    fclose(file);
    
return 0;
}





