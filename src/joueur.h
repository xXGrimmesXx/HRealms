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
    void soigner(int montant);
    void defausser(int n);
    void assomerChampions();
    void assomerChampion(Champion* c);
    // TODO
    void sacrifier(int montant, Partie& p);

    // interface Attaquable
    void prendreDegats(int m) override;
    Joueur* proprietaire() override { return this; }

    // getters et setters
    std::string getNom() const { return nom; }
    Main& getMain() { return main; }
    Deck& getDeck() { return deck; }
    Pile& getDefausse() { return defausse; }
    std::vector<Champion*>& getChampions() { return champions; }
    const std::vector<Champion*>& getChampions() const { return champions; }
    void setPv(int v) { pv = v; }
    int getPv() const { return pv; }
};
