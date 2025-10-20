#pragma once
#include "../gameView.h"
#include "../../model/faction.h"
#include "../../model/tour.h"
#include "../../model/pile.h"


#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <string.h>
#include <stdio.h>


// Codes ANSI pour les couleurs
#define RESET   "\033[0m"
#define BLACK   "\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// Couleurs vives
#define BRIGHT_BLACK   "\033[90m"
#define BRIGHT_RED     "\033[91m"
#define BRIGHT_GREEN   "\033[92m"
#define BRIGHT_YELLOW  "\033[93m"
#define BRIGHT_BLUE    "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN    "\033[96m"
#define BRIGHT_WHITE   "\033[97m"

// Styles
#define BOLD    "\033[1m"
#define DIM     "\033[2m"
#define ITALIC  "\033[3m"
#define UNDERLINE "\033[4m"

class ConsoleView : public GameView {
public:
    ConsoleView();
    ~ConsoleView();
    
    // Implémentation des méthodes d'affichage
        void afficherMessage(std::string& message)override;
        void afficherErreur(std::string& erreur)override;
        void afficherPartie(Partie& partie)override;
        void afficherJoueur(Joueur& joueur)override;
        void afficherMain(Joueur& joueur)override;
        void afficherMarche(Marche& marche)override;
        void afficherScores(std::vector<Joueur>& joueurs)override;
        void afficherGagnant(Joueur& gagnant)override;
        void afficherPileGemmeDeFeu(const Pile& pile);
        void afficherDefausse(const Joueur& joueur);


    // Implémentation des méthodes de saisie
    int demanderNombreJoueurs() override;
    std::string demanderNomJoueur(int numero) override;
    int demanderChoixAction() override;
    std::vector<int> demanderIndexCarte() override;
    int demanderIndexJoueur(std::vector<Joueur>& joueurs) override;
    int demanderIndexChampion(Joueur& joueur) override;
    int demanderSlotMarche() override;
    bool confirmerAction(std::string& action) override;
    void attendreEntree() override;
    void viderEcran() override;

private:
    std::string getFactionSymbol(Faction faction) ;
};
