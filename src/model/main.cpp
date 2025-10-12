#include "main.h"

#include "Carte.h"
#include "joueur.h"

void Main::ajouter(Carte* c){
    cartes.push_back(c);
}

void Main::defausser(int idx){
    if(idx < 0 || idx >= (int)cartes.size()) {
        throw std::out_of_range("Index hors limites");
    }
    delete cartes[idx];
    cartes.erase(cartes.begin() + idx);
}

void Main::viderMain(Joueur* j){
    for(Carte* c : cartes) {
        delete c;
        j->getDefausse().ajouter(c);

    }
    cartes.clear();
}

Carte* Main::getCarte(int idx) const{
    if(idx < 0 || idx >= (int)cartes.size()) {
        throw std::out_of_range("Index hors limites");
    }
    return cartes[idx];
}

void Main::toString() const{
    std::cout << "Main: \n";
    int i=1;
    for(const Carte* c : cartes) {
        std::cout <<  i << ") " << c->getNom() << " ";
        ++i;
    }
    std::cout << std::endl;
}

bool Main::contientCarteDeFaction(Faction f){
    for(auto c : cartes){
        if(c->getFaction()==f){
            return true;
        }
    }
    return false;
}
void Main::sacrifier(Carte* c){
    for (int i=0; i<(int)cartes.size(); i++){
        if (cartes.at(i)==c){
            //itérateur pointant sur la carte à supprimer
            cartes.erase(std::next(cartes.begin(),i));
        }
    }
}

