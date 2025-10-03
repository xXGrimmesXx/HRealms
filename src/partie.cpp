#include "Partie.h"
#include <iostream>

Partie::Partie() : nb_tour(0), idx_joueur_actuel(0), tour_courant(nullptr) {}

void Partie::commencer() {
    // init joueurs, decks, marché
}

void Partie::jouerTour() {
    // logiquement : commencerTour -> jouer -> finTour
}

void Partie::finTour() {
    // reset, passer au joueur suivant
}

bool Partie::estTerminee() const {
    // un joueur a 0 PV ?
    return false;
}

void Partie::toggleGodMode() {
    godMode.actif = !godMode.actif;
}

void Partie::GOD_mettreTousJoueurs1PV() {
    for (auto& j : joueurs) j.setPv(1);
}

void Partie::GOD_acheterDepuisDeckMarche(int idx, Joueur& j) {
    // implementation god mode
}
