#pragma once
#include <deque>
#include <algorithm>
#include <random>

#include "Carte.h"
#include "pile.h"

class Deck {
private:
    std::deque<Carte*> cartes;
    std::default_random_engine rng ;
public:
    // Constructeur & destructeur
    Deck();
    ~Deck();

    // Getters
    int getTaille() const { return cartes.size(); }
    std::vector<Carte*> getCartes() {
        return std::vector<Carte*>(cartes.begin(), cartes.end());
    }
    
    // Méthodes
    void melanger();
    Carte* piocher();
    void ajouterDessous(Carte* c);
    bool estVide() const;
    void rechargerDepuis(Pile& defausse);


};
