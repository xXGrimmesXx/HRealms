#include "Partie.h"
#include <iostream>

#include "Tour.h"

Partie::Partie():joueurs(),marche(new Marche()),tour_courant(nullptr),nb_tour(0),idx_joueur_actuel(0),godMode(GodMode()),cartesSacrifie(),gemmeDeFeu() {

}

void Partie::commencer(std::vector<std::string> nomsJoueurs) {
    // Initialisation de la partie
    for (size_t i = 0; i < nomsJoueurs.size(); i++) {
        Joueur* j = new Joueur(nomsJoueurs.at(i));
        joueurs.push_back(j);
    }
    marche->remplirLigne();
    nb_tour = 1;
    idx_joueur_actuel = 0;
    tour_courant = new Tour(joueurs[0]);
}

void Partie::commencerTour() {
    Joueur* joueurActuel = joueurs[idx_joueur_actuel];
    tour_courant = new Tour(joueurActuel);
}

void Partie::finTour() {
    Joueur* joueurActuel = joueurs[idx_joueur_actuel];
    joueurActuel->finTour();
    idx_joueur_actuel = (idx_joueur_actuel + 1) % joueurs.size();
    if (idx_joueur_actuel == 0) {
        nb_tour++;
    }
    tour_courant = nullptr;
}

bool Partie::estTerminee() const {
    for (const auto& j : joueurs) {
        if (j->getPv() <= 0) {
            return true;
        }
    }
    return false;
}

void Partie::toggleGodMode() {
    godMode.actif = !godMode.actif;
}

void Partie::GOD_mettreTousJoueurs1PV() {
    for (auto& j : joueurs) j->setPv(1);
}

void Partie::GOD_acheterDepuisDeckMarche(int idx, Joueur* j) {
    marche->acquerirDepuisDeck(idx, j);
}

Joueur* Partie::getGagnant() {
    for (int i = 0; i < (int)joueurs.size(); ++i) {
        if (joueurs[i]->getPv() <= 0) {
            return joueurs[(i - 1) % joueurs.size()]; // Le joueur suivant gagne
        }
    }
    return nullptr;
}

std::string Partie::toString() const {
    std::string res = "Partie avec " + std::to_string(joueurs.size()) + " joueurs, tour " + std::to_string(nb_tour) + "\n";
    for (const auto& j : joueurs) {
        res += j->toString() + "\n";
    }
    return res;
}

std::string Partie::toStringDetail() const {
    std::string res = "Partie avec " + std::to_string(joueurs.size()) + " joueurs, tour " + std::to_string(nb_tour) + "\n";
    for (auto j : joueurs) {
        res += j->toString() + "\n";
        res += "  Defausse: " + std::to_string(j->getDefausse()->getTaille()) + " cartes\n";
        res += "  Main: " + std::to_string(j->getMain()->getTaille()) + " cartes\n";
        res += "  Champions: " + std::to_string(j->getChampions().size()) + " champions\n";
    }
    return res;
}
std::ostream& operator<<(std::ostream& os, const Partie& p) {
    os << p.toString();
    return os;
}

// Setters pour la restauration depuis JSON
void Partie::setCartesSacrifie(Pile* p) {
    if (!p) return;
    
    // Vider la pile actuelle sans supprimer les cartes
    while (cartesSacrifie.getTaille() > 0) {
        cartesSacrifie.prendreDessus();
    }
    
    // Copier les cartes de la nouvelle pile (transfert de propriété)
    for (auto carte : p->getCartes()) {
        cartesSacrifie.ajouter(carte);
    }
}

void Partie::setGemmeDeFeu(GemmeDeFeu* g) {
    if (!g) return;
    
    // Remplacer la gemme de feu actuelle
    gemmeDeFeu = *g;
}

void Partie::setMarche(Marche* m) {
    if (!m) return;
    
    // Supprimer l'ancien marché
    if (marche) {
        delete marche;
    }
    
    // Assigner le nouveau marché (transfert de propriété)
    marche = m;
}
