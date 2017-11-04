/*
 * \file global.c
 * \author IBIS Ibrahim
 * \date 3 novembre 2017
 *
 * Gestion des données globales de la trace
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/global.h"

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
        nouveau->delai_moyen=0;
        nouveau->nb_evenement=0;
        nouveau->temps_attente_files=0;
        nouveau->temps_transmission_liens=0;
        nouveau->nb_paquet_perdus_temps=0;
        nouveau->intervalle=1; // Intervalle des pertes
        nouveau->nb_flux_actif=0;
        nouveau->ecart_type_dm=0;

        nouveau->nb_paquet_all_file=0;
        nouveau->nb_paquet_transit=0;

        return nouveau;
}

Trace newTrace(void){
        return (Trace) NULL;
}


void freeGlobal(GlobalData g){
        free(g);

}

void insertionData(GlobalData data,Trace t,Param params){
        data->nb_evenement++;
        switch (t->code)
        {
        case 0:
                data->nb_paquet_emis++;
                data->nb_paquet_all_file++;
                data->nb_paquet_transit++;
                break;
        case 1:
                data->nb_paquet_all_file++;
                break;
        case 2:
                data->nb_paquet_traites++;
                data->nb_paquet_all_file--;
                break;
        case 3:
                data->nb_paquet_recus++;
                data->nb_paquet_all_file--;
                data->nb_paquet_transit--;
                break;
        case 4:
                data->nb_paquet_perdus++;
                data->nb_paquet_perdus_temps++;
                if ( t->t > data->intervalle )
                {
                        if ( params->perdus )
                                fprintf(params->fileperdus, "%f %d \n", t->t, data->nb_paquet_perdus_temps );
                        data->nb_paquet_perdus_temps = 0;
                        data->intervalle++;

                }


                data->nb_paquet_all_file--;
                data->nb_paquet_transit--;
                break;
        default:

                break;
        }

        if ( strcmp(params->noeud, "all") == 0 )
        {
                if ( (precision == 0) )
                {
                        fprintf(params->fileatt, "%f %d \n", t->t, data->nb_paquet_all_file );
                }
                precision=(precision+1)%250;
        }

        if ( params->transit && t->code != 1 && t->code !=2 )
        {
                if ( (precision2 == 0) )
                {
                        fprintf(params->filetransit, "%f %d \n", t->t, data->nb_paquet_transit );
                }
                precision2=(precision2+1)%500;
        }

}

void affichage_donnees_globales(GlobalData g)
{

        printf("Nombre evenements : %d \n",g->nb_evenement);
        printf("Nombre paquets traités : %d \n",g->nb_paquet_traites);
        printf("Nombre paquets émis : %d \n",g->nb_paquet_emis);
        printf("Nombre paquets reçus : %d \n",g->nb_paquet_recus);
        printf("Nombre de paquets perdus : %d \n",g->nb_paquet_perdus);
        g->taux_perte = (float)g->nb_paquet_perdus/(float)g->nb_paquet_emis*100;
        printf("Taux de perte : %f %% \n\n",g->taux_perte);

        printf("Nombre de flux : %d \n",g->nb_flux);
        printf("Nombre de noeud : %d \n\n",g->nb_noeud);



        g->temps_attente_files = (float)g->temps_attente_files/(float)g->nb_paquet_recus;
        g->temps_transmission_liens = (float)g->temps_transmission_liens/(float)g->nb_paquet_recus;
        printf("Temps moyen transmission sur les liens : %f \n",g->temps_transmission_liens);
        printf("Temps moyen d'attente dans les files : %f \n",g->temps_attente_files );
        g->delai_moyen =(float)g->delai_moyen/(float)g->nb_paquet_recus;
        printf("Délai moyen de bout en bout  : %f \n",g->delai_moyen );



}
