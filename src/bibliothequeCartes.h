#ifndef BIBLIOTHEQUECARTES_H
#define BIBLIOTHEQUECARTES_H
#include "Deck.h"

class BibliothequeCartes {
public:
    static Deck creerDeckDepart();
    static Deck creerDeckMarche();
    static Pile creerPileGemmeDeFeu();
};

#endif