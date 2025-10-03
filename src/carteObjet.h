#pragma once
#include "Carte.h"

class CarteObjet : public Carte {
public:
    void lorsDeJeu(Joueur& j, Partie& p) override;
};
