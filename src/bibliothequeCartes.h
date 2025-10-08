#ifndef BIBLIOTHEQUECARTES_H
#define BIBLIOTHEQUECARTES_H
#include "deck.h"
#include "pile.h"
#include "carteAction.h"
#include "carteObjet.h"
#include "champion.h"
#include "effet.h"
#include "faction.h"
#include "typeEffet.h"
#include "typeCible.h"
#include <string>

class BibliothequeCartes {
public:
    static Deck creerDeckDepart();
    static Deck creerDeckMarche();
    static Pile creerPileGemmeDeFeu();
};

#endif