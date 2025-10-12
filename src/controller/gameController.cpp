#include "gameController.h"
#include "../model/joueur.h"
#include "../model/marche.h"
#include "../model/carte.h"
#include <algorithm>
#include <stdexcept>

GameController::GameController(GameView* gameView) 
    : partie(nullptr), view(gameView), jeuEnCours(false) {
    if (!view) {
        throw std::invalid_argument("GameView ne peut pas être null");
    }
}

GameController::~GameController() {
    // unique_ptr se charge automatiquement de la destruction
}

void GameController::lancerJeu() {
    std::string message = "Bienvenue dans Hero Realms !";
    view->afficherMessage(message);
    
    try {
        initialiserPartie();
        boucleJeu();
        finirPartie();
    } catch (const std::exception& e) {
        std::string message = "Erreur fatale: " + std::string(e.what());
        view->afficherErreur(message);
    }
}

void GameController::initialiserPartie() {
    // Demander le nombre de joueurs
    int nombreJoueurs = view->demanderNombreJoueurs();
    
    // Demander les noms des joueurs (vérifier unicité)
    std::vector<std::string> nomsJoueurs;
    for (int i = 1; i <= nombreJoueurs; ++i) {
        std::string nom;
        do {
            nom = view->demanderNomJoueur(i);
            // Vérifier si ce nom est déjà utilisé
            auto it = std::find(nomsJoueurs.begin(), nomsJoueurs.end(), nom);
            if (it != nomsJoueurs.end()) {
                std::string msg = "Ce nom de joueur est déjà pris. Veuillez en choisir un autre.";
                view->afficherErreur(msg);
                nom.clear();
            }
        } while (nom.empty());
        nomsJoueurs.push_back(nom);
    }
    
    // Créer la partie
    partie = std::make_unique<Partie>();
    partie->commencer(nomsJoueurs);
    
    jeuEnCours = true;
    std::string message = "Partie initialisée avec " + std::to_string(nombreJoueurs) + " joueurs !";
    view->afficherMessage(message);
    view->attendreEntree();
}

void GameController::boucleJeu() {
    while (jeuEnCours && !partie->estTerminee()) {
        jouerTour();
    }
}

void GameController::jouerTour() {
    try {
        // Commencer le tour
        partie->jouerTour();
        
        // Afficher l'état du jeu
        afficherEtatJeu();
        
        // Gérer les actions du joueur
        bool tourTermine = false;
        while (!tourTermine && !partie->estTerminee()) {
            gererActionJoueur();
            
            // Vérifier si le joueur veut finir son tour
            // Cette logique dépendra de votre implémentation
            // Pour l'instant, on suppose qu'une action termine le tour
            tourTermine = true;
        }
        
        // Finir le tour
        if (!partie->estTerminee()) {
            partie->finTour();
        }
        
    } catch (const std::exception& e) {
        std::string message = "Erreur pendant le tour: " + std::string(e.what());
        gererErreur(message);
    }
}

void GameController::gererActionJoueur() {
    int choix = view->demanderChoixAction();
    
    switch (choix) {
        case 1: // Jouer une carte
            if (!jouerCarte()) {
                std::string message = "Impossible de jouer cette carte.";
                view->afficherErreur(message);
            }
            break;
            
        case 2: // Acheter une carte
            if (!acheterCarte()) {
                std::string message = "Impossible d'acheter cette carte.";
                view->afficherErreur(message);
            }
            break;
            
        case 3: // Attaquer un joueur
            if (!attaquerJoueur()) {
                std::string message = "Impossible d'attaquer ce joueur.";
                view->afficherErreur(message);
            }
            break;
            
        case 4: // Finir le tour
            finirTour();
            break;
            
        default:
            std::string message = "Choix invalide. Veuillez choisir entre 1 et 6.";
            view->afficherErreur(message);
            break;
    }
}

bool GameController::jouerCarte() {
    Joueur& joueurActuel = partie->getJoueurActuel();
    
    // Afficher la main
    view->afficherMain(joueurActuel);
    
    // Demander quelle carte jouer
    int index = view->demanderIndexCarte();
    
    // Valider l'index
    if (!validerIndex(index,joueurActuel.getMain().getTaille(), "carte")) {
        return false;
    }
    
    // Essayer de jouer la carte
    try {
        bool succes = joueurActuel.jouerCarte(index, *partie);
        if (succes) {
            std::string message = "Carte jouée avec succès !";
            view->afficherMessage(message);
        }
        return succes;
    } catch (const std::exception& e) {
        std::string message = "Erreur lors du jeu de carte: " + std::string(e.what());
        view->afficherErreur(message);
        return false;
    }
}

bool GameController::acheterCarte() {
    Marche* marche = partie->getMarche();
    
    // Afficher le marché
    view->afficherMarche(*marche);
    
    // Demander quelle carte acheter
    int slot = view->demanderSlotMarche();
    
    // Valider le slot (supposons 5 slots dans le marché)
    if (!validerIndex(slot, 5, "slot du marché")) {
        return false;
    }
    
    // Essayer d'acheter la carte
    try {
        bool succes = partie->getJoueurActuel().acheterDepuisMarche(slot, *partie);
        if (succes) {
            std::string message = "Carte achetée avec succès !";
            view->afficherMessage(message);
        }
        return succes;
    } catch (const std::exception& e) {
        std::string message = "Erreur lors de l'achat: " + std::string(e.what());
        view->afficherErreur(message);
        return false;
    }
}

bool GameController::attaquerJoueur() {
    std::vector<Joueur>& joueurs = partie->getJoueurs();
    const Joueur& joueurActuel = partie->getJoueurActuel();
    
    // Afficher les joueurs disponibles
    std::string message = "Joueurs disponibles pour l'attaque:";
    view->afficherMessage(message);
    for (size_t i = 0; i < joueurs.size(); ++i) {
        if (joueurs[i].getNom() != joueurActuel.getNom()) {
            view->afficherJoueur(joueurs[i]);
        }
    }
    
    // Demander quel joueur attaquer
    int index = view->demanderIndexCarte(); // Réutiliser cette méthode
    
    // Valider l'index
    if (!validerIndex(index, static_cast<int>(joueurs.size()), "joueur")) {
        return false;
    }
    
    // Confirmer l'attaque
    std::string nomCible = joueurs[index].getNom();
    message = "Confirmez-vous l'attaque contre " + nomCible + " ?";
    if (!view->confirmerAction(message)) {
        return false;
    }
    
    // Essayer d'attaquer
    try {
        Joueur& cible = partie->getJoueurs()[index];
        if (cible.aGardeMobilise()) {
            std::string message = nomCible + " a un garde mobilisé. Attaque annulée.";
            view->afficherErreur(message);
            return false;
        }
        int degats = partie->getTourCourant()->getDegats();
        if (degats > cible.getPv()) {
            degats = cible.getPv(); // Ne pas infliger plus de dégâts que les PV restants
        }
        bool succes = cible.prendreDegats(degats);
        if (succes) {
            partie->getTourCourant()->utiliserDegats(degats); // Réinitialiser les dégâts après l'attaque
            message = "Attaque réussie contre " + nomCible + " !";
            view->afficherMessage(message);
        }
        return succes;
    } catch (const std::exception& e) {
        std::string message = "Erreur lors de l'attaque: " + std::string(e.what());
        view->afficherErreur(message);
        return false;
    }
}

void GameController::finirTour() {
    std::string message = "Êtes-vous sûr de vouloir finir votre tour ?";
    if (view->confirmerAction(message)) {
        std::string message = "Tour terminé.";
        view->afficherMessage(message);
        // La logique de fin de tour sera gérée dans la boucle principale
    }
}

void GameController::afficherEtatJeu() {
    view->afficherPartie(*partie);
    view->afficherScores(partie->getJoueurs());
    afficherMarche();
    afficherMain();
}

void GameController::afficherMain() {
    Joueur& joueurActuel = partie->getJoueurActuel();
    view->afficherMain(joueurActuel);
}

void GameController::afficherMarche() {
    Marche* marche = partie->getMarche();
    view->afficherMarche(*marche);
}

void GameController::finirPartie() {
    if (partie && partie->estTerminee()) {
        Joueur* gagnant = partie->getGagnant();
        if(gagnant!=nullptr){
            view->afficherGagnant(*gagnant);
            std::string message = "Merci d'avoir joué à Hero Realms !";
            view->afficherMessage(message);
            jeuEnCours = false;
        }
        else{
            std::string message = "La partie s'est terminée sans gagnant clair.";
            view->afficherMessage(message);
            jeuEnCours = false;
        }

        
    }
    
    
}

bool GameController::validerIndex(int index, int max, const std::string& type) {
    if (index < 0 || index >= max) {
        std::string message = "Index de " + type + " invalide. Doit être entre 1 et " + std::to_string(max) + ".";
        view->afficherErreur(message);
        return false;
    }
    return true;
}

void GameController::gererErreur(std::string& erreur) {
    view->afficherErreur(erreur);
    view->attendreEntree();
}