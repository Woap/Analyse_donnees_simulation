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

        // TABLEAU
        //printf("Heure : %f \n",temps_attente_files);

        // TABLEAU
        //printf("Heure : %f \n",temps_transmission_liens);

        // TABLEAU
        //printf("Heure : %f \n",taille_files_attente);

}
