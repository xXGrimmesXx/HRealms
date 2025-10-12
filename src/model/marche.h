#pragma once
#include <vector>
#include "Deck.h"
#include "Pile.h"
#include "Carte.h"
#include "joueur.h"
#include "partie.h"


class Marche {
private:
    Deck* deckMarche;
    std::vector<Carte*> ligne;
    Pile pileGemmesFeu;
    Pile zoneSacrifice;

public:
    void remplirLigne();
    bool acquerirDepuisLigne(int slot, Joueur& j, Partie& partie);
    bool acquerirGemmeFeu(Joueur& j, Partie& partie);
    bool acquerirDepuisDeck(int idx, Joueur& j, Partie& partie);
    void retournerGemmeFeuDansPile(Carte* g);
};
