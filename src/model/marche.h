#pragma once
#include <vector>
#include "Deck.h"
#include "Pile.h"
#include "Carte.h"
#include "joueur.h"
#include "partie.h"
#include "bibliothequeCartes.h"
#include "tour.h"


class Marche {
private:
    Deck deckMarche;
    std::vector<Carte*> ligne;
    Pile pileGemmesFeu;
    Pile zoneSacrifice;

public:
    Marche();
    void remplirLigne();
    bool acquerirDepuisLigne(int slot, Joueur& j, Partie& partie);
    bool acquerirGemmeFeu(Joueur& j, Partie& partie);
    bool acquerirDepuisDeck(int idx, Joueur& j, Partie& partie);
    void retournerGemmeFeuDansPile(Carte* g);

    const std::vector<Carte*>& getLigne() const { return ligne; }
};
