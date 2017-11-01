#ifndef GLOBAL_H
#define GLOBAL_H


typedef struct s_globaldata
{

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

Trace newTrace(void);
GlobalData newData(void);
void insertionData(GlobalData data,Trace t);
void affichage_donnees_globales(GlobalData g);

#endif
