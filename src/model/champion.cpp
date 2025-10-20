#include "champion.h"
#include "joueur.h"
#include "partie.h"


void Champion::mobiliser(Partie& p,std::vector<void*> target) {
    if (!mobilise) {
        mobilise = true;
        if (target.size() != effetsActiver.size()) {
            throw std::runtime_error("Number of targets does not match number of activation effects");
            return;
        }
        for (size_t i = 0; i < effetsActiver.size(); ++i) {
            effetsActiver.at(i).appliquer(this, target.at(i), p);
        }
    } else {
        throw std::runtime_error("Champion already mobilized");
    }
}
void Champion::demobiliser(){
    mobilise = false;
}