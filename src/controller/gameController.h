#pragma once
#include "../model/partie.h"
#include "../view/gameView.h"
#include <memory>

class GameController {
private:
    std::unique_ptr<Partie> partie;
    GameView* view;
    bool jeuEnCours;

public:
    GameController(GameView* gameView);
    ~GameController();
    
    // Gestion du jeu principal
    void lancerJeu();
    void initialiserPartie();
    void boucleJeu();
    void finirPartie();
    
    // Actions de tour
    void jouerTour();
    void gererActionJoueur();
    
    // Actions spécifiques
    bool jouerCarte();
    bool acheterCarte();
    bool attaquerJoueur();
    void finirTour();
    
    // Actions d'affichage
    void afficherEtatJeu();
    void afficherMain();
    void afficherMarche();
    
    // Utilitaires
    bool validerIndex(int index, int max, const std::string& type);
    void gererErreur(const std::string& erreur);
};