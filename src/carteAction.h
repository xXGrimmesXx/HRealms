#pragma once
#include "Carte.h"

class CarteAction : public Carte {
public:
    void lorsDeJeu(Joueur& j, Partie& p) override;
};
