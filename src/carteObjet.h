#pragma once
#include "Carte.h"

class CarteObjet : public Carte {
public:
    // Constructeur
    CarteObjet(std::string nom, int cout, Faction f) : Carte(nom, cout, f) {}
    void lorsDeJeu(Joueur& j, Partie& p) override;
};
