#include "Partie.h"
#include <iostream>

#include "Tour.h"

Partie::Partie():joueurs(),marche(new Marche()),tour_courant(nullptr),nb_tour(0),idx_joueur_actuel(0),godMode(GodMode()),cartesSacrifie(),gemmeDeFeu() {}

void Partie::commencer(std::vector<std::string> nomsJoueurs, ModeJeu mode) {
    // Initialisation de la partie
    for (size_t i = 0; i < nomsJoueurs.size(); i++) {
        Joueur* j = new Joueur(nomsJoueurs.at(i));
        joueurs.push_back(j);
    }
    marche->remplirLigne();
    nb_tour = 1;
    auto rd = std::random_device {};
    auto rng = std::default_random_engine {rd()};
    std::uniform_int_distribution<size_t> dist(0, joueurs.size() - 1);
    idx_joueur_actuel = dist(rng);
    tour_courant = new Tour(joueurs.at(idx_joueur_actuel));

    // si plus de deux joueurs le premier joueur commence à trois cartes etc...
    if (joueurs.size() > 2) {
        for (int i=2;i>0;i--) {
            Joueur * joueur = joueurs.at((idx_joueur_actuel + 2-i) % joueurs.size());
            Main * m = joueur->getMain();
            //fait defausser deux cartes pour le premier joueur 1 carte pour le deuxieme et 0 pour les autres
            for (int j=0; j<i;j++){
                Carte * c = m->getCartes().at(0);
                m->defausser(c);
                joueur->getDeck()->ajouterDessous(c);
            }
        }
    }
    if (joueurs.size() == 2 && mode != ModeJeu::CLASSIQUE) {
        throw std::invalid_argument("Seul le mode classique peut être joué à deux joueurs"); 
    }
    else if (joueurs.size() >=3 ) {
        modeJeu = mode;
    }
}

void Partie::commencerTour() {
    Joueur* joueurActuel = joueurs.at(idx_joueur_actuel);
    tour_courant = new Tour(joueurActuel);
}

void Partie::finTour() {
    Joueur* joueurActuel = joueurs.at(idx_joueur_actuel);
    joueurActuel->finTour();
    idx_joueur_actuel = (idx_joueur_actuel + 1) % joueurs.size();
    if (idx_joueur_actuel == 0) {
        nb_tour++;
    }
    tour_courant = nullptr;
}

bool Partie::estTerminee() const {
    if (modeJeu == ModeJeu::CLASSIQUE || modeJeu == ModeJeu::PREMIER_SANG) {
        for (const auto& j : joueurs) {
            if (j->getPv() <= 0) {
                return true;
            }
        }
        return false;
    }
    if (joueurs.size() == 1) {
        return true;
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
    if (modeJeu == ModeJeu::CLASSIQUE || modeJeu == ModeJeu::PREMIER_SANG) {
        for (int i = 0; i < (int)joueurs.size(); ++i) {
            if (joueurs.at(i)->getPv() <= 0) {
                return joueurs.at((i + 1) % joueurs.size()); // Le joueur suivant gagne
            }
        }
    }
    return joueurs.at(0);
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