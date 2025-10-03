#pragma once
#include "Carte.h"

class Champion : public Carte {
public:
    void lorsDeJeu(Joueur& j, Partie& p) override;
};
