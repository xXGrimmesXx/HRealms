#pragma once
#include <vector>
#include "Joueur.h"
#include "Marche.h"
#include "gemmeDeFeu.h"
#include "modeJeu.h"

#include "GodMode.h"

class Tour;
class Marche;

class Partie {
private:
    std::vector<Joueur*> joueurs;
    Marche* marche;
    Tour* tour_courant;
    int nb_tour;
    int idx_joueur_actuel;
    GodMode godMode;
    Pile cartesSacrifie;
    GemmeDeFeu gemmeDeFeu;
    ModeJeu modeJeu;

public:
    Partie();
    void commencer(std::vector<std::string> nomsJoueurs, ModeJeu mode);
    void commencerTour();
    void finTour();
    bool estTerminee() const;
    void toggleGodMode();
    void GOD_mettreTousJoueurs1PV();
    void GOD_acheterDepuisDeckMarche(int idx, Joueur* j);

    //getters setters
    Tour* getTourCourant() const { return tour_courant; }
    Joueur* getJoueurActuel() { return joueurs.at(idx_joueur_actuel);}
    int getIdxJoueurActuel(){return idx_joueur_actuel;}
    Marche* getMarche() { return marche; }
    GodMode* getGodMode() { return &godMode; }
    int getNbTour() const { return nb_tour; }
    std::vector<Joueur*> getJoueurs() { return joueurs; }
    ModeJeu getModeJeu() const { return modeJeu; }
    void setIdxJoueurActuel(int idx) { idx_joueur_actuel = idx; };
    void setNbTour(int n) { nb_tour = n; }
    void setTourCourant(Tour* t) { tour_courant = t; }
    void ajouterJoueur(Joueur* j) { joueurs.push_back(j); }
    void setJoueurs(const std::vector<Joueur*>& j) { joueurs = j; }
    Pile* getCartesSacrifie() {return &cartesSacrifie;}
    GemmeDeFeu* getGemmeDeFeu(){return &gemmeDeFeu;}
    void setCartesSacrifie(Pile* p);
    void setGemmeDeFeu(GemmeDeFeu* g);
    void setMarche(Marche* m);
    void setGodMode(bool actif) { godMode.actif = actif; }
    void setModeJeu(ModeJeu mode) { modeJeu = mode; }

    Joueur* getGagnant();

    std::string toString() const ;
    std::string toStringDetail() const;
    friend std::ostream& operator<<(std::ostream& os, const Partie& p);

};
