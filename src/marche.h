#pragma once
#include <vector>
#include "Deck.h"
#include "Pile.h"
#include "Carte.h"
#include "joueur.h"
#include "partie.h"

class Deck;
class Carte;
class Joueur;
class Partie;

class Marche {
private:
    Deck* deckMarche;
    std::vector<Carte*> ligne;
    Pile pileGemmesFeu;
    Pile zoneSacrifice;

public:
    void remplirLigne();
    void acquerirDepuisLigne(int slot, Joueur& j, Partie& partie);
    void acquerirGemmeFeu(Joueur& j, Partie& partie);
    void acquerirDepuisDeck(int idx, Joueur& j, Partie& partie);
    void retournerGemmeFeuDansPile(Carte* g);
};
