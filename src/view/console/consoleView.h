#pragma once
#include "../gameView.h"
#include "../../model/faction.h"
#include "../../model/tour.h"
#include "../../model/pile.h"
#include "../../model/typeEffet.h"
#include "../../model/typeCible.h"
#include "../../model/deck.h"


#include <iostream>
#include <iomanip>
#include <limits>
#include <string>
#include <string.h>
#include <stdio.h>


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
    void afficherChampions(const std::vector<Champion*>& champions)override;
    void afficherDefausse(const Joueur& joueur)override;
    void afficherDeckMarche(Deck& deckMarche)override;

    //helpers
    std::string typeEffetToString(TypeEffet t);
    std::string typeCibleToString(TypeCible c);

    // Implémentation des méthodes de saisie
    int demanderNombreJoueurs() override;
    std::string demanderNomJoueur(int numero) override;
    int demanderChoixAction(Partie& partie) override;
    std::vector<int> demanderIndexCarte() override;
    int demanderIndexJoueur(std::vector<Joueur>& joueurs) override;
    int demanderIndexJoueur(std::vector<Joueur>& joueurs, int joueurActuelIndex) override;
    std::vector<int> demanderIndexChampion(Joueur& joueur) override;
    int demanderSlotMarche() override;
    bool confirmerAction(std::string& action) override;
    void attendreEntree() override;
    void viderEcran() override;
    int demanderChoixGodMode()override;
    int demanderIndexDeck(Deck& deckMarche)override;
    std::string demanderMotDePasseGodMode()override;
    


private:
    std::string getFactionSymbol(Faction faction) ;
};
