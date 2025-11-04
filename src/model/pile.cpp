#include "pile.h"
#include <algorithm>
#include <random>

#include "carte.h"



Pile::~Pile() {
    for (Carte* c : cartes) {
        delete c;
    }
}
void Pile::ajouter(Carte* c) {
    cartes.push_back(c);
}
Carte* Pile::prendreDessus() {
    if (cartes.empty()) {
        return nullptr;
    }
    Carte* c = cartes.back();
    cartes.pop_back();
    return c;
}
int Pile::getTaille() const {
    return cartes.size();
}

std::vector<Carte*> Pile::getCartes(){
    return cartes;
}

const std::vector<Carte*> Pile::getCartes() const {
    return cartes;
}


bool Pile::estVide() const {
    return cartes.empty();
}

Carte* Pile::retirerCarte(int index) {
    if (index < 0 || index >= (int)cartes.size()) {
        return nullptr;
    }
    Carte* carte = cartes[index];
    cartes.erase(cartes.begin() + index);
    return carte;
}
