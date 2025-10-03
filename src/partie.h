#pragma once
#include <vector>
#include "Joueur.h"
#include "Marche.h"
#include "Tour.h"
#include "GodMode.h"

class Partie {
private:
    std::vector<Joueur> joueurs;
    Marche marche;
    Tour* tour_courant;
    int nb_tour;
    int idx_joueur_actuel;
    GodMode godMode;

public:
    Partie();
    void commencer();
    void jouerTour();
    void finTour();
    bool estTerminee() const;
    void toggleGodMode();
    void GOD_mettreTousJoueurs1PV();
    void GOD_acheterDepuisDeckMarche(int idx, Joueur& j);
};
