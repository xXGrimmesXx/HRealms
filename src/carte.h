#pragma once
#include <string>
#include <vector>
#include "Faction.h"
#include "Effet.h"

class Joueur;
class Partie;

class Carte {
protected:
    int id;
    std::string nom;
    int cout;
    Faction faction;
    std::vector<Effet> effetsPrimaire;

public:
    virtual ~Carte() = default;
    virtual void lorsDeJeu(Joueur& j, Partie& p) = 0;
};
