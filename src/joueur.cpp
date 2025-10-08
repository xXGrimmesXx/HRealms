#include "Joueur.h"

Joueur::Joueur(std::string nom="") : nom(nom), pv(50), deck(), defausse(), main(), champions() {}

void Joueur::piocher(int n){
    for (int i = 0; i < n; ++i) {
        if (deck.estVide()) {
            // Mélanger la défausse dans le deck
            while (defausse.taille() > 0) {
                Carte* c = defausse.prendreDessus();
                deck.ajouterCarte(c);
            }
            deck.melanger();
        }
        Carte* c = deck.piocher();
        if (c != nullptr) {
            main.ajouter(c);
        }
    }
}
void Joueur::jouerCarte(int idx){
    if (idx < 0 || idx >= main.taille()) {
        throw std::out_of_range("Index de carte invalide");
    }
    Carte* c = main.prendreCarte(idx);
    if (c != nullptr) {
        c->lorsDeJeu(*this, /* partie */); // Vous devez passer la partie actuelle ici
        defausse.ajouter(c);
    }
}
void Joueur::acheterDepuisMarche(int slot){}
void Joueur::acheterGemmeFeu(){}
void Joueur::resoudreDegats(int reserve, Attaquable* cible){}
void Joueur::finTour(){}
bool Joueur::aGardeMobilise(){ return false; }
void Joueur::soigner(int montant){}