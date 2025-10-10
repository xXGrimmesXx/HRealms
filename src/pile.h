#pragma once
#include <vector>
#include "carte.h"

class Carte;

class Pile {
private:
    std::vector<Carte*> cartes;
public:
    Pile()=default;
    ~Pile();
    void ajouter(Carte* c);
    Carte* prendreDessus();
    int taille() const;
    void melanger();
};
