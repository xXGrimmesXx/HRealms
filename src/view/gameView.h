#pragma once
#include <string>
#include <vector>

// Forward declarations
class Partie;
class Joueur;
class Marche;
class Carte;

class GameView {
public:
    virtual ~GameView() = default;
    
    // Interface pour l'affichage
        virtual void afficherMessage(std::string& message) = 0;
        virtual void afficherErreur(std::string& erreur) = 0;
        virtual void afficherPartie(Partie& partie) = 0;
        virtual void afficherJoueur(Joueur& joueur) = 0;
        virtual void afficherMain(Joueur& joueur) = 0;
        virtual void afficherMarche(Marche& marche) = 0;
        virtual void afficherScores(std::vector<Joueur>& joueurs) = 0;
        virtual void afficherGagnant(Joueur& gagnant) = 0;

    // Interface pour les saisies utilisateur
    virtual int demanderNombreJoueurs() = 0;
    virtual std::string demanderNomJoueur(int numero) = 0;
    virtual int demanderChoixAction() = 0;
    virtual int demanderIndexCarte() = 0;
    virtual int demanderIndexJoueur(std::vector<Joueur>& joueurs) = 0;
    virtual int demanderIndexChampion(Joueur& joueur) = 0;
    virtual int demanderSlotMarche() = 0;
        virtual bool confirmerAction(std::string& action) = 0;
    virtual void attendreEntree() = 0;
    virtual void viderEcran() = 0;
};
