#include "gemmeDeFeu.h"
#include "joueur.h"
#include "partie.h"
#include "bibliothequeCartes.h"

GemmeDeFeu::GemmeDeFeu(): pile(BibliothequeCartes::creerPileGemmeDeFeu()) {
}

bool GemmeDeFeu::acheter(Joueur* joueur, Partie* partie) {
    if (!estDisponible()) return false;
    if (partie->getTourCourant()->getOr() < COUT) return false;

    Carte* gemme = pile.prendreDessus();
    if (gemme) {
        joueur->getDefausse()->ajouter(gemme);
        partie->getTourCourant()->utiliserOr(COUT);
        return true;
    }
    return false;
}

bool GemmeDeFeu::estDisponible() const {
    return !pile.estVide();
}

int GemmeDeFeu::getNombreDisponible() const {
    return pile.getTaille();
}


// Setter pour la restauration depuis JSON
void GemmeDeFeu::setPile(Pile* p) {
    pile = *p;
}
