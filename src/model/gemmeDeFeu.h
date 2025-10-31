#pragma once
#include "pile.h"

class Joueur;
class Partie;

class GemmeDeFeu {
private:
    Pile pile;
    static const int COUT = 2;

public:
    GemmeDeFeu();
    ~GemmeDeFeu() = default;
    
    bool acheter(Joueur* joueur, Partie* partie);
    bool estDisponible() const;

    // Getters
    int getNombreDisponible() const;
    int getCout() const { return COUT; }
    
    Pile* getPile() { return &pile; }
    const Pile* getPile() const { return &pile; }
    
    // Setter pour la restauration depuis JSON
    void setPile(Pile* p);
};