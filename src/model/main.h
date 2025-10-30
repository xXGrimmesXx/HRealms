#pragma once
#include <vector>
#include "faction.h"
#include <string>
#include <iostream>

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
    
    bool contientCarteDeFaction(Faction f);
    bool contientCarteEffetSacrifice();
    void sacrifier(Carte* c);

    std::string toString() const;
    friend std::ostream& operator<<(std::ostream& os, const Main& m){os<<m.toString(); return os; }
};
