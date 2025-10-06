#pragma once
#include "Carte.h"

class CarteAction : public Carte {
private:
    int gainOr;
    int gainDegats;
public:
    // Constructeur
    CarteAction(int id, std::string nom, int cout, Faction f, int or, int degats);

    // Méthode
    void lorsDeJeu(Joueur& j, Partie& p) override;
};
