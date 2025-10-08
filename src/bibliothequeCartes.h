#ifndef BIBLIOTHEQUECARTES_H
#define BIBLIOTHEQUECARTES_H
#include "Deck.h"
#include "Pile.h"
#include "CarteAction.h"
#include "CarteObjet.h"
#include "Champion.h"
#include "Effet.h"
#include "faction.h"
#include <string>

class BibliothequeCartes {
public:
    static Deck creerDeckDepart();
    static Deck creerDeckMarche();
    static Pile creerPileGemmeDeFeu();
};

#endif