#pragma once
#include <vector>
#include "Carte.h"

class Carte;

class Main{
private:
    std::vector<Carte*> cartes;
public:
    // Constructeur & destructeur
    Main()=default;
    ~Main();

    // Getters
    Carte* getCarte(int idx) const;
    const std::vector<Carte*>& getCartes() const { return cartes; }
    int getTaille() const { return cartes.size(); }

    // Méthodes
    void ajouter(Carte* c);
    void defausser(int idx);
    void viderMain();
    void toString() const;
};
