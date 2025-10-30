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
    cartes.at(idx)->setEstJouee(false);
    cartes.erase(cartes.begin() + idx);
}

void Main::viderMain(Joueur* j){
    for(Carte* c : cartes) {
        c->setEstJouee(false);
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

std::string Main::toString() const{
    std::string result = "Main: ";
    int i=1;
    for(const Carte* c : cartes) {
        result += std::to_string(i) + ") " + c->getNom() + " ";
        ++i;
    }
    return result;
}

bool Main::contientCarteDeFaction(Faction f){
    for(auto c : cartes){
        if(c->getFaction()==f){
            return true;
        }
    }
    return false;
}

bool Main::contientCarteEffetSacrifice() {
    for (auto c : cartes) {
        if (!c->getEffetsSacrifice().empty()) {
            return true;
        }
    }
    return false;
}

void Main::sacrifier(Carte* c) {
    for (auto it = cartes.begin(); it != cartes.end(); ++it) {
        if (*it == c) {
            c->setEstJouee(false);
            cartes.erase(it);
            return;
        }
    }
}



