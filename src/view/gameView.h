#pragma once
#include <string>
#include <vector>


// Forward declarations
class Partie;
class Joueur;
class Marche;
class Carte;
class Pile;
class Champion;
class Deck;
class Effet;

class GameView {
public:
    virtual ~GameView() = default;
    
    // Interface pour l'affichage
        virtual void afficherMessage(std::string& message) = 0;
        virtual void afficherErreur(std::string& erreur) = 0;
        virtual void afficherPartie(Partie* partie) = 0;
        virtual void afficherJoueur(Joueur* joueur) = 0;
        virtual void afficherMain(Joueur* joueur) = 0;
        virtual void afficherMarche(Marche* marche) = 0;
        virtual void afficherScores(std::vector<Joueur*> joueurs) = 0;
        virtual void afficherGagnant(Joueur* gagnant) = 0;
        virtual void afficherPileGemmeDeFeu(std::vector<Carte*> pile) = 0;
        virtual void afficherChampions(std::vector<Champion*> champions) = 0;
        virtual void afficherDefausse(Joueur* joueur) = 0;
        virtual void afficherDeckMarche(Deck* deckMarche) = 0;
        virtual void afficherDefausseChampion(Joueur* joueur) = 0;
        virtual void afficherCartesEffetSacrifice(std::vector<Carte*> cartes) = 0;
        virtual void afficherPileSacrifice(Partie* partie) = 0;

    // Interface pour les saisies utilisateur
    virtual int demanderNombreJoueurs() = 0;
    virtual std::string demanderNomJoueur(int numero) = 0;
    virtual int demanderChoixAction(Partie* partie) = 0;
    virtual std::vector<int> demanderIndexCarte() = 0;
    virtual int demanderIndexJoueur(std::vector<Joueur*> joueurs) = 0;
    virtual int demanderIndexJoueur(std::vector<Joueur*> joueurs, int joueurActuelIndex) = 0;
    virtual std::vector<int> demanderIndexChampion(Joueur* joueur) = 0;
    virtual std::vector<int> demanderIndexGarde(Joueur* joueur) = 0;
    virtual int demanderIndexCarteEffetSacrifice() = 0;
    virtual int demanderSlotMarche() = 0;
    virtual bool confirmerAction(std::string& action) = 0;
    virtual void attendreEntree() = 0;
    virtual void viderEcran() = 0;
    virtual int demanderChoixGodMode() = 0;
    virtual int demanderIndexDeck(Deck* deckMarche) = 0;
    virtual std::string demanderMotDePasseGodMode() = 0;
    virtual bool demanderChoix(Effet effet) = 0;
    virtual int demanderChoixInitialisationPartie() = 0;
    virtual std::string demanderNomFichierSauvegarde() = 0;
};
