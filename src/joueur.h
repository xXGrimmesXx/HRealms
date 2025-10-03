#pragma once
#include <string>
#include <vector>
#include "Deck.h"
#include "Pile.h"
#include "Main.h"
#include "Champion.h"
#include "Attaquable.h"

class Joueur : public Attaquable {
private:
    std::string nom;
    int pv;
    Deck deck;
    Pile defausse;
    Main main;
    std::vector<Champion*> champions;

public:
    Joueur(std::string nom="");
    void piocher(int n);
    void jouerCarte(int idx);
    void acheterDepuisMarche(int slot);
    void acheterGemmeFeu();
    void resoudreDegats(int reserve, Attaquable* cible);
    void finTour();
    bool aGardeMobilise() const;

    // interface Attaquable
    void prendreDegats(int m) override;
    Joueur* proprietaire() override { return this; }

    void setPv(int v) { pv = v; }
    int getPv() const { return pv; }
};
