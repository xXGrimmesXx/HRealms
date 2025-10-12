#pragma once
#include <vector>
#include "faction.h"

class Joueur;
class Carte;

class Main{
private:
    std::vector<Carte*> cartes;
public:
    // Constructeur & destructeur
    Main()=default;
    ~Main()=default;

    // Getters
    Carte* getCarte(int idx) const;
    const std::vector<Carte*>& getCartes() const { return cartes; }
    int getTaille() const { return cartes.size(); }

    // Méthodes
    void ajouter(Carte* c);
    void defausser(int idx);
    void viderMain(Joueur* j);
    void toString() const;
    bool contientCarteDeFaction(Faction f);
    void sacrifier(Carte* c);
};
