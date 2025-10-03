#pragma once
#include "TypeEffet.h"
#include "TypeCible.h"

class Joueur;
class Partie;

class Effet {
private:
    TypeEffet type;
    int montant;
    TypeCible cible;

public:
    Effet(TypeEffet t, int m, TypeCible c);
    void appliquer(Joueur* src, void* tgt, Partie& partie);
};
