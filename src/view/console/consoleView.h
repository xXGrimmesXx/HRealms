#pragma once
#include "../gameView.h"
#include "../../model/faction.h"
#include "../../model/tour.h"

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

    // Implémentation des méthodes de saisie
    int demanderNombreJoueurs() override;
    std::string demanderNomJoueur(int numero) override;
    int demanderChoixAction() override;
    int demanderIndexCarte() override;
    int demanderIndexJoueur(std::vector<Joueur>& joueurs) override;
    int demanderIndexChampion(Joueur& joueur) override;
    int demanderSlotMarche() override;
    bool confirmerAction(std::string& action) override;
    void attendreEntree() override;
    void viderEcran() override;

private:
    std::string getFactionSymbol(Faction faction) ;
};
