#include "Partie.h"
#include <iostream>

#include "Tour.h"

Partie::Partie() : joueurs(std::vector<Joueur>(0)),marche(new Marche()),tour_courant(nullptr), nb_tour(0), idx_joueur_actuel(0), godMode(GodMode()),cartesSacrifie(Pile()) {
}

void Partie::commencer(std::vector<std::string> nomsJoueurs) {
    // Initialisation de la partie
    for (const auto& nom : nomsJoueurs) {
        joueurs.push_back(Joueur(nom));
    }
    marche = new Marche();
    nb_tour = 1;
    idx_joueur_actuel = 0;
    tour_courant = new Tour(&joueurs[0]);
    std::cout << "La partie commence !" << std::endl;
}

void Partie::jouerTour() {
    Joueur& joueurActuel = joueurs[idx_joueur_actuel];
    if (tour_courant == nullptr || tour_courant->getJoueur() != &joueurActuel) {
        if (tour_courant != nullptr) {
            delete tour_courant; // Libérer la mémoire de l'ancien tour
        }
        tour_courant = new Tour(&joueurActuel);
    }
    std::cout << "Tour " << nb_tour << " - Joueur: " << joueurActuel.getNom() << std::endl;
    joueurActuel.piocher(5);
    while(joueurActuel.getMain().getTaille() > 0) {
        std::cout << "Cartes en main: " << joueurActuel.getMain().getTaille() << std::endl;
        std::cout << "Or disponible: " << tour_courant->getOr() << ", Points de combat disponibles: " << tour_courant->getDegats() << std::endl;
        std::cout << "Que voulez-vous faire ? (1: Jouer une carte, 2: Acheter une carte du marché, 3: Acheter une gemme de feu, 4: Finir le tour) ";
        int choix;
        std::cin >> choix;
        if (choix == 1) {
            int idx;
            std::cout << "Index de la carte à jouer (0-" << joueurActuel.getMain().getTaille()-1 << "): ";
            std::cin >> idx;
            joueurActuel.jouerCarte(idx,*this);
        } else if (choix == 2) {
            int slot;
            std::cout << "Slot du marché à acheter (0-4): ";
            std::cin >> slot;
            joueurActuel.acheterDepuisMarche(slot,*this);
        } else if (choix == 3) {
            joueurActuel.acheterGemmeFeu(*this);
        } else if (choix == 4) {
            break;
        } else {
            std::cout << "Choix invalide, veuillez réessayer." << std::endl;
        }
    }
    finTour();
}

void Partie::finTour() {
    Joueur& joueurActuel = joueurs[idx_joueur_actuel];
    joueurActuel.finTour();
    tour_courant->reinitialiser();
    idx_joueur_actuel = (idx_joueur_actuel + 1) % joueurs.size();
    if (idx_joueur_actuel == 0) {
        nb_tour++;
    }
}

bool Partie::estTerminee() const {
    for (const auto& j : joueurs) {
        if (j.getPv() <= 0) {
            std::cout << "Le joueur " << j.getNom() << " a perdu !" << std::endl;
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
