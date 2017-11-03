/*
 * \file global.h
 * \author IBIS Ibrahim
 * \date 3 novembre 2017
 *
 * Declarations des fonctions et structures
 *
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#include <stdio.h>

int precision;
int precision1;
int precision2;
int precision3;

typedef struct s_globaldata
{

        int nb_paquet_traites;
        int nb_paquet_emis;
        int nb_paquet_recus;
        int nb_flux;
        int nb_paquet_perdus;
        int nb_evenement;
        int nb_noeud;

        int nb_paquet_all_file; // Nombre de paquets dans l'ensemble des files
        int nb_paquet_transit; // Nombre de paquet en transit
        int nb_flux_actif; // Nombre de flux actif

        float taux_perte;
        float delai_moyen;
        float temps_attente_files;
        float temps_transmission_liens;


} sglobalData, *GlobalData;

typedef struct s_trace {
        float t; // heure de l'evenement
        int code; // type de l'evenement
        int pid; // identifiant du paquet
        int fid; // identifidant du flux auquel appartient le paquet
        char s[1024]; // noeud source du paquet
        char d[1024]; // noeud destination du paquet
        char pos[1024]; // position actuel sauf code = 2 prochain saut

} p_trace, *Trace;

typedef struct s_param {
        // Options utilisateurs
        char noeud[1024];
        int flux_delai;
        int transit;
        int flux_actif;
        int perdus;

        FILE *fileatt; // params->noeud
        FILE *filefluxactif;
        FILE *fileflux_delai;
        FILE *filetransit;
        FILE *fileperdus;


} p_param, *Param;

Trace newTrace(void);
GlobalData newData(void);
void freeGlobal(GlobalData g);
void insertionData(GlobalData data,Trace t, Param params);
void affichage_donnees_globales(GlobalData g);

#endif
