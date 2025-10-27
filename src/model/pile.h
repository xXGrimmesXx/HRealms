#pragma once
#include <vector>


class Carte;

class Pile {
private:
    std::vector<Carte*> cartes;
public:
    // Constructeur & destructeur
    Pile()=default;
    ~Pile();
    
    // Getters
    int getTaille() const;
    std::vector<Carte*> getCartes();
    const std::vector<Carte*>& getCartes() const;

    // Méthodes
    void ajouter(Carte* c);
    Carte* prendreDessus();
    void melanger();
    bool estVide() const;
    Carte* retirerCarte(int idx);
};
