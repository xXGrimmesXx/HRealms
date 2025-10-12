#pragma once

#include "joueur.h"

class Attaquable {
public:
    virtual ~Attaquable() = default;
    virtual bool prendreDegats(int m) = 0;
    virtual Joueur* proprietaire() = 0;
};
