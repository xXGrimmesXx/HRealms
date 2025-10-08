#pragma once
#include "TypeEffet.h"
#include "TypeCible.h"
#include "joueur.h"
#include "partie.h"

#include <stdexcept>

class Joueur;
class Partie;

class Effet {
private:
    TypeEffet type;
    int montant;
    TypeCible cible;

public:
    Effet(TypeEffet t, int m, TypeCible c);
    void appliquer(Joueur* src, void* tgt, Partie& partie,int choix);

    TypeEffet getTypeEffet(){return type;}
    int getMontant(){return montant;}
    TypeCible getTypeCible(){return cible;}

    void setTypeEffet(TypeEffet t){type = t;}
    void setMontant(int m){montant = m;}
    void setTypeCible(TypeCible c){cible = c;}


};
