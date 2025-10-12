#pragma once
#include "TypeEffet.h"
#include "TypeCible.h"


#include <iostream>
#include <stdexcept>

class Joueur;
class Partie;
class Carte;

class Effet {
private:
    TypeEffet type;
    int montant1;
    int montant2;
    TypeCible cible;

public:
    Effet(TypeEffet t, int m1, int m2, TypeCible c);
    Effet(TypeEffet t, int m1, TypeCible c);
    void appliquer(Carte* src, void* tgt, Partie& partie) const;

    TypeEffet getTypeEffet()const{return type;}
    int getMontant1()const{return montant1;}
    int getMontant2()const{return montant2;}
    TypeCible getTypeCible()const{return cible;}

    void setTypeEffet(TypeEffet t){type = t;}
    void setMontant1(int m){montant1 = m;}
    void setMontant2(int m){montant2 = m;}
    void setTypeCible(TypeCible c){cible = c;}


};
