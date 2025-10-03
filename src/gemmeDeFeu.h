#pragma once
#include "Carte.h"

class GemmeDeFeu : public Carte {
public:
    void lorsDeJeu(Joueur& j, Partie& p) override;
};
