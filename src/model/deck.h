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
    Deck();
    ~Deck();
    void melanger();
    Carte* piocher();
    void ajouterDessous(Carte* c);
    bool estVide() const;
    void rechargerDepuis(Pile& defausse);
};
