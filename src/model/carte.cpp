#include "Carte.h"
#include "Joueur.h"
#include "Partie.h"
#include "Effet.h"

// Constructeur
Carte::Carte( const std::string nom, int cout, Faction f) : nom(nom), cout(cout), faction(f) {
    static int compteurId = 0;
    id = compteurId;
    compteurId++;
}

// Méthodes


void Carte::sacrifier(Joueur& j, Partie& p) {
    // Effet reste à définir
    for (const auto& effet : effetsSacrifice) {
        effet.appliquer(this, {}, p, false);
    }
    j.getMain().sacrifier(this);
    p.getCartesSacrifie().ajouter(this);
}

void Carte::lorsDeJeu(Partie& p, std::vector<ChoixEffet*> target, std::vector<ChoixEffet*> allies,bool declencherAllie) {
    for (size_t i = 0; i < effetsPrimaire.size(); ++i) {
        effetsPrimaire.at(i).appliquer(this, target.at(i), p, false);
    }
    if (declencherAllie) {
        for (size_t i = 0; i < effetsAllie.size(); ++i) {
            effetsAllie.at(i).appliquer(this, allies.at(i), p, false);
        }
    }
    setEstJouee(true);
}