#pragma once
#include <deque>
#include <algorithm>
#include <random>

#include "Carte.h"
#include "pile.h"

class Deck {
private:
    std::deque<Carte*> cartes;
public:
    Deck();
    ~Deck();
    void melanger();
    Carte* piocher();
    void ajouterDessous(Carte* c);
    bool estVide() const;
    void rechargerDepuis(Pile& defausse);
};
