#include "Partie.h"
#include <iostream>

#include "Tour.h"

Partie::Partie(){
    joueurs = std::vector<Joueur>();
    marche = new Marche();
    tour_courant = nullptr;
    nb_tour = 0;
    idx_joueur_actuel = 0;
    godMode = GodMode();
    cartesSacrifie = Pile();
}

void Partie::commencer(std::vector<std::string> nomsJoueurs) {
    // Initialisation de la partie
    for (size_t i = 0; i < nomsJoueurs.size(); i++) {
        Joueur j(nomsJoueurs.at(i));
        joueurs.push_back(j);
    }
    marche->remplirLigne();
    nb_tour = 1;
    idx_joueur_actuel = 0;
    tour_courant = new Tour(&joueurs[0]);
}

void Partie::jouerTour() {
    Joueur& joueurActuel = joueurs[idx_joueur_actuel];
    tour_courant = new Tour(&joueurActuel);
    if (tour_courant == nullptr || tour_courant->getJoueur() != &joueurActuel) {
        if (tour_courant != nullptr) {
            tour_courant = nullptr;
        }
        tour_courant = new Tour(&joueurActuel);
    }
    std::cout << "Nouveau tour pour " << joueurActuel.getNom() << " (Tour " << nb_tour << ")" << std::endl;
}

void Partie::finTour() {
    Joueur& joueurActuel = joueurs[idx_joueur_actuel];
    joueurActuel.finTour();
    joueurActuel.piocher(5);
    idx_joueur_actuel = (idx_joueur_actuel + 1) % joueurs.size();
    if (idx_joueur_actuel == 0) {
        nb_tour++;
    }
}

bool Partie::estTerminee() const {
    for (const auto& j : joueurs) {
        if (j.getPv() <= 0) {
            return true;
        }
    }
    return false;
}

void Partie::toggleGodMode() {
    godMode.actif = !godMode.actif;
}

void Partie::GOD_mettreTousJoueurs1PV() {
    for (auto& j : joueurs) j.setPv(1);
}

void Partie::GOD_acheterDepuisDeckMarche(int idx, Joueur& j) {
    tour_courant->ajouterOr(100); // Donner suffisamment d'or pour l'achat
    marche->acquerirDepuisDeck(idx, j, *this);
}

Joueur* Partie::getGagnant() {
    for (int i = 0; i < (int)joueurs.size(); ++i) {
        if (joueurs[i].getPv() <= 0) {
            return &joueurs[(i - 1) % joueurs.size()]; // Le joueur suivant gagne
        }
    }
    return nullptr;
}

std::string Partie::toString() const {
    std::string res = "Partie avec " + std::to_string(joueurs.size()) + " joueurs, tour " + std::to_string(nb_tour) + "\n";
    for (const auto& j : joueurs) {
        res += j.toString() + "\n";
    }
    return res;
}

std::string Partie::toStringDetail() const {
    std::string res = "Partie avec " + std::to_string(joueurs.size()) + " joueurs, tour " + std::to_string(nb_tour) + "\n";
    for (auto j : joueurs) {
        res += j.toString() + "\n";
        res += "  Defausse: " + std::to_string(j.getDefausse().taille()) + " cartes\n";
        res += "  Main: " + std::to_string(j.getMain().getTaille()) + " cartes\n";
        res += "  Champions: " + std::to_string(j.getChampions().size()) + " champions\n";
    }
    return res;
}
std::ostream& operator<<(std::ostream& os, const Partie& p) {
    os << p.toString();
    return os;
}
