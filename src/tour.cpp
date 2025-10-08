#include "tour.h"

Tour::Tour(Joueur* j) : joueur(j), reserveOr(0), reserveDegats(0) {}
void Tour::ajouterOr(int m) {
    reserveOr += m;
}

void Tour::ajouterDegats(int m) {
    reserveDegats += m;
}

void Tour::reinitialiser() {
    reserveOr = 0;
    reserveDegats = 0;
}

int Tour::getOr() const {
    return reserveOr;
}

int Tour::getDegats() const {
    return reserveDegats;
}

Joueur* Tour::getJoueur() const {
    return joueur;
}

void Tour::setJoueur(Joueur* j) {
    joueur = j;
}

