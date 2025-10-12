#include "gameController.h"
#include "../model/joueur.h"
#include "../model/marche.h"
#include "../model/carte.h"
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
    view->afficherMessage("Bienvenue dans Hero Realms !");
    
    try {
        initialiserPartie();
        boucleJeu();
        finirPartie();
    } catch (const std::exception& e) {
        view->afficherErreur("Erreur fatale: " + std::string(e.what()));
    }
}

void GameController::initialiserPartie() {
    // Demander le nombre de joueurs
    int nombreJoueurs = view->demanderNombreJoueurs();
    
    // Demander les noms des joueurs
    std::vector<std::string> nomsJoueurs;
    for (int i = 1; i <= nombreJoueurs; ++i) {
        std::string nom = view->demanderNomJoueur(i);
        nomsJoueurs.push_back(nom);
    }
    
    // Créer la partie
    partie = std::make_unique<Partie>();
    partie->commencer(nomsJoueurs);
    
    jeuEnCours = true;
    view->afficherMessage("Partie initialisée avec " + std::to_string(nombreJoueurs) + " joueurs !");
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
        gererErreur("Erreur pendant le tour: " + std::string(e.what()));
    }
}

void GameController::gererActionJoueur() {
    int choix = view->demanderChoixAction();
    
    switch (choix) {
        case 1: // Jouer une carte
            if (!jouerCarte()) {
                view->afficherErreur("Impossible de jouer cette carte.");
            }
            break;
            
        case 2: // Acheter une carte
            if (!acheterCarte()) {
                view->afficherErreur("Impossible d'acheter cette carte.");
            }
            break;
            
        case 3: // Attaquer un joueur
            if (!attaquerJoueur()) {
                view->afficherErreur("Impossible d'attaquer ce joueur.");
            }
            break;
            
        case 4: // Finir le tour
            finirTour();
            break;
            
        case 5: // Afficher ma main
            afficherMain();
            break;
            
        case 6: // Afficher le marché
            afficherMarche();
            break;
            
        default:
            view->afficherErreur("Choix invalide. Veuillez choisir entre 1 et 6.");
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
            view->afficherMessage("Carte jouée avec succès !");
        }
        return succes;
    } catch (const std::exception& e) {
        view->afficherErreur("Erreur lors du jeu de carte: " + std::string(e.what()));
        return false;
    }
}

bool GameController::acheterCarte() {
    const Marche* marche = partie->getMarche();
    
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
            view->afficherMessage("Carte achetée avec succès !");
        }
        return succes;
    } catch (const std::exception& e) {
        view->afficherErreur("Erreur lors de l'achat: " + std::string(e.what()));
        return false;
    }
}

bool GameController::attaquerJoueur() {
    const std::vector<Joueur>& joueurs = partie->getJoueurs();
    const Joueur& joueurActuel = partie->getJoueurActuel();
    
    // Afficher les joueurs disponibles
    view->afficherMessage("Joueurs disponibles pour l'attaque:");
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
    if (!view->confirmerAction("Attaquer " + nomCible)) {
        return false;
    }
    
    // Essayer d'attaquer
    try {
        bool succes = partie->attaquerJoueur(index);
        if (succes) {
            view->afficherMessage("Attaque réussie contre " + nomCible + " !");
        }
        return succes;
    } catch (const std::exception& e) {
        view->afficherErreur("Erreur lors de l'attaque: " + std::string(e.what()));
        return false;
    }
}

void GameController::finirTour() {
    if (view->confirmerAction("Êtes-vous sûr de vouloir finir votre tour")) {
        view->afficherMessage("Tour terminé.");
        // La logique de fin de tour sera gérée dans la boucle principale
    }
}

void GameController::afficherEtatJeu() {
    view->afficherPartie(*partie);
    view->afficherScores(partie->getJoueurs());
}

void GameController::afficherMain() {
    const Joueur& joueurActuel = partie->getJoueurActuel();
    view->afficherMain(joueurActuel);
    view->attendreEntree();
}

void GameController::afficherMarche() {
    const Marche& marche = partie->getMarche();
    view->afficherMarche(marche);
    view->attendreEntree();
}

void GameController::finirPartie() {
    if (partie && partie->estTerminee()) {
        const Joueur& gagnant = partie->getGagnant();
        view->afficherGagnant(gagnant);
    }
    
    view->afficherMessage("Merci d'avoir joué à Hero Realms !");
    jeuEnCours = false;
}

bool GameController::validerIndex(int index, int max, const std::string& type) {
    if (index < 0 || index >= max) {
        view->afficherErreur("Index de " + type + " invalide. Doit être entre 1 et " + std::to_string(max) + ".");
        return false;
    }
    return true;
}

void GameController::gererErreur(const std::string& erreur) {
    view->afficherErreur(erreur);
    view->attendreEntree();
}