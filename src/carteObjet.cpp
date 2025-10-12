#include "carteObjet.h"
#include "joueur.h"

void CarteObjet::lorsDeJeu(Joueur& j, Partie& p) {
    for (const Effet& effet : effetsPrimaire) {
        effet.appliquer(this, &j, p);
    }
    
    for (const Effet& effet : effetsAllie) {
        int nbAllies=0;
        for(Champion* c : j.getChampions()){
            if(c->getFaction() == this->getFaction()){
                nbAllies++;
            }
        }
        for (Carte* c : j.getMain().getCartes()) {
            if (c->getFaction() == this->getFaction() && c != this) {
                nbAllies++;
            }
        }
        effet.appliquer(this, &j, p);
    }
}