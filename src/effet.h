#pragma once
#include "TypeEffet.h"
#include "TypeCible.h"
#include "joueur.h"
#include "partie.h"

#include <iostream>
#include <stdexcept>

class Joueur;
class Partie;

class Effet {
private:
    TypeEffet type;
    int montant1;
    int montant2;
    TypeCible cible;

public:
    Effet(TypeEffet t, int m1, int m2, TypeCible c);
    void appliquer(Joueur* src, void* tgt, Partie& partie,int choix) const;

    TypeEffet getTypeEffet(){return type;}
    int getMontant1(){return montant1;}
    int getMontant2(){return montant2;}
    TypeCible getTypeCible(){return cible;}

    void setTypeEffet(TypeEffet t){type = t;}
    void setMontant1(int m){montant1 = m;}
    void setMontant2(int m){montant2 = m;}
    void setTypeCible(TypeCible c){cible = c;}


};
