#include "Joueur.h"

Joueur(std::string nom="") : nom(nom), pv(50), deck(), defausse(), main(), champions() {}

void piocher(int n){
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
void jouerCarte(int idx){
    if (idx < 0 || idx >= main.taille()) {
        throw std::out_of_range("Index de carte invalide");
    }
    Carte* c = main.prendreCarte(idx);
    if (c != nullptr) {
        c->lorsDeJeu(*this, /* partie */); // Vous devez passer la partie actuelle ici
        defausse.ajouter(c);
    }
}
void acheterDepuisMarche(int slot);
void acheterGemmeFeu();
void resoudreDegats(int reserve, Attaquable* cible);
void finTour();
bool aGardeMobilise();
void soigner(int montant);