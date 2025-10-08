#pragma once
#include "Carte.h"

class CarteAction : public Carte {
public:
    // Constructeur
    CarteAction(std::string nom, int cout, Faction f);

    // Méthode
    void lorsDeJeu(Joueur& j, Partie& p) override;
};
