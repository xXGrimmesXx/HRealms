#pragma once
#include "Joueur.h"

class Tour {
private:
    Joueur* joueur;
    int reserveOr;
    int reserveDegats;

public:
    Tour(Joueur* j);
    void ajouterOr(int m);
    void ajouterDegats(int m);
    void reinitialiser();
    int getOr() const;
    int getDegats() const;
    Joueur* getJoueur() const;
    void setJoueur(Joueur* j);
};
