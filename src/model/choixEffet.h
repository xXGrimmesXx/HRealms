#pragma once
#include "effet.h"

class ChoixEffet {
public:
    ChoixEffet(void* cible,bool choix)
        : cible(cible), choix(choix) {}

    void* getCible() const { return cible; }
    bool getChoix() const { return choix; }

private:
    void* cible;
    bool choix;
};
