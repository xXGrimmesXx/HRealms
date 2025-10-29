#include "champion.h"
#include "joueur.h"
#include "partie.h"


void Champion::mobiliser(Partie& p,std::vector<ChoixEffet*> target) {
    if (!mobilise) {
        mobilise = true;
        if (target.size() != effetsActiver.size()) {
            throw std::runtime_error("Number of targets does not match number of activation effects");
            return;
        }
        for (size_t i = 0; i < effetsActiver.size(); ++i) {
            std::cout << "Application de l'effet d'activation " << i << std::endl;
            std::cout << "Type d'effet : " << static_cast<int>(effetsActiver.at(i).getTypeEffet()) << std::endl;
            effetsActiver.at(i).appliquer(this, target.at(i)->getCible(), p, target.at(i)->getChoix());
        }
    } else {
        throw std::runtime_error("Champion already mobilized");
    }
}
void Champion::demobiliser(){
    mobilise = false;
}