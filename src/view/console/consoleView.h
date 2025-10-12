#pragma once
#include "../gameView.h"
#include "../../model/faction.h"

class ConsoleView : public GameView {
public:
    ConsoleView();
    ~ConsoleView() override;
    
    // Implémentation des méthodes d'affichage
    void afficherMessage(const std::string& message) override;
    void afficherErreur(const std::string& erreur) override;
    void afficherPartie(const Partie& partie) override;
    void afficherJoueur(const Joueur& joueur) override;
    void afficherMain(const Joueur& joueur) override;
    void afficherMarche(const Marche& marche) override;
    void afficherScores(const std::vector<Joueur>& joueurs) override;
    void afficherGagnant(const Joueur& gagnant) override;
    
    // Implémentation des méthodes de saisie
    int demanderNombreJoueurs() override;
    std::string demanderNomJoueur(int numero) override;
    int demanderChoixAction() override;
    int demanderIndexCarte() override;
    int demanderSlotMarche() override;
    bool confirmerAction(const std::string& action) override;
    void attendreEntree() override;
    void viderEcran() override;

private:
    std::string getFactionSymbol(Faction faction) const;
};
