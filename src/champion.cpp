#include "champion.h"


Champion::Champion(const std::string& nom, int cout, Faction f, bool garde, int vie) : Carte(nom, cout, f) {
    this->vie = vie;
    this->garde = garde;
    this->mobilise = false;
}

Champion::~Champion() {}

void Champion::lorsDeJeu(Joueur& j, Partie& p) {
    for (const Effet& effet : effetsPrimaire) {
        effet.appliquer(&j, &j, p,0);
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
        effet.appliquer(&j, &j, p,nbAllies*bonusParAllie);
    }
}

void Champion::mobiliser(Joueur& j, Partie& p) {
    if (!mobilise) {
        mobilise = true;
        for (const Effet& effet : effetsActiver) {
            effet.appliquer(&j, &j, p,0);
        }
    } else {
        throw std::runtime_error("Champion already mobilized");
    }
}