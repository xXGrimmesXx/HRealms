#pragma once
#include <vector>
#include "Carte.h"

class Main{
private:
    std::vector<Carte*> cartes;
public:
    // Constructeur & destructeur
    Main()=default;
    ~Main();

    // Méthodes
    void ajouter(Carte* c);
    void defausser(int idx);
    void viderMain();
    void jouerCarte(int idx);
    void toString() const;
};
