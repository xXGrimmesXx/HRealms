#pragma once
#include "Carte.h"

class Champion : public Carte {

    int defense;
    bool garde; // Si vrai, le champion doit être attaqué avant les autres champions ou le joueur
    std::vector<Effet> effetsActiver;

public:

    // Constructeur
    Champion(const std::string& nom, int cout, Faction f, int combat, bool garde);

    // Getters & setters
    std::vector<Effet> getEffetsActiver() const {return effetsActiver;}
    Carte* ajouterEffetActiver(const Effet& e) { effetsActiver.push_back(e); return this; }

    // Méthodes
    void lorsDeJeu(Joueur& j, Partie& p) override;
};
