#pragma once
#include <vector>


class Carte;

class Pile {
private:
    std::vector<Carte*> cartes;
public:
    Pile()=default;
    ~Pile();

    // Méthodes
    void ajouter(Carte* c);
    Carte* prendreDessus();
    int getTaille() const;
    void melanger();
    std::vector<Carte*> getCartes();
    bool estVide() const;
};
