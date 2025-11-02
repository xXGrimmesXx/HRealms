#include "main.h"

#include "Carte.h"
#include "joueur.h"

void Main::ajouter(Carte* c){
    cartes.push_back(c);
}

void Main::defausser(Carte * c){
    if(c == nullptr) {
        throw std::invalid_argument("Carte invalide");
    }
    for (auto carte : cartes) {
        if (carte == c) {
            carte->setEffetAllieActive(false);
            carte->setEffetPrimaireActive(false);
            cartes.erase(std::remove(cartes.begin(), cartes.end(), carte), cartes.end());
            return;
        }
    }
    
}

void Main::viderMain(Joueur* j){
    for(Carte* c : cartes) {
        c->setEffetAllieActive(false);
        c->setEffetPrimaireActive(false);
        j->getDefausse()->ajouter(c);

    }
    cartes.clear();
}

Carte* Main::getCarte(int idx) const{
    if(idx < 0 || idx >= (int)cartes.size()) {
        return nullptr;
    }
    return cartes.at(idx);
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
    // Retire la carte de la main
    for (auto i=0; i<(int)cartes.size(); i++) {
        if (cartes.at(i) == c) {
            cartes.at(i)->setEffetAllieActive(false);
            cartes.at(i)->setEffetPrimaireActive(false);
            cartes.erase(cartes.begin() + i);
            return;
        }
    }
}



