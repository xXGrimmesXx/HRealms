#include "pile.h"
#include <algorithm>
#include <random>

Pile::Pile() {
    cartes = std::vector<Carte*>();
}

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
        return nullptr; // or throw an exception
    }
    Carte* c = cartes.back();
    cartes.pop_back();
    return c;
}
int Pile::taille() const {
    return cartes.size();
}

void Pile::melanger() {
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cartes.begin(), cartes.end(), g);
}
