#pragma once
#include "../model/partie.h"
#include "../view/gameView.h"
#include "../model/tour.h"
#include "../model/choixEffet.h"
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
    bool mobiliserChampion();
    bool acheterCarte();
    bool acheterGemmeDeFeu();
    bool attaquerJoueur();
    bool consulterDefausse();
    void finirTour();
    std::vector<ChoixEffet*> choisirCible(Carte* carte,std::vector<Effet> effets);
    ChoixEffet* demanderChoixEffet(Effet effet, Partie& partie, Carte* carteSource);
    
    // Actions d'affichage
    void afficherEtatJeu();
    void afficherMain();
    void afficherMarche();
    void afficherPileGemmeDeFeu();
    void afficherChampions();
    void afficherMainAdversaire();
    
    // Utilitaires
    Joueur* getJoueurParIndex(int index);
    bool validerIndex(int index, int max, const std::string& type);
    void gererErreur(std::string& erreur);
    void toggleGodMode();
    void executerActionGodMode(int choix);
};