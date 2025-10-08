#pragma once
#include <deque>
#include "Carte.h"

class Pile;

class Deck {
private:
    std::deque<Carte*> cartes;
public:
    Deck() = default;
    ~Deck();
    void melanger();
    Carte* piocher();
    void ajouterDessous(Carte* c);
    bool estVide() const;
    void rechargerDepuis(Pile& defausse);
};
