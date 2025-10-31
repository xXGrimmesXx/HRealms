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


void Carte::sacrifier(std::vector<ChoixEffet*> cibles, Partie* p) {
    // Effet reste à définir
    for (int i = 0; i < (int)effetsSacrifice.size(); ++i) {
        effetsSacrifice.at(i).appliquer(this, cibles.at(i)->getCible(), p, cibles.at(i)->getChoix());
    }
}

void Carte::jouerEffetPrimaires(Partie* p, std::vector<ChoixEffet*> target) {
    for (size_t i = 0; i < effetsPrimaire.size(); ++i) {
        effetsPrimaire.at(i).appliquer(this, target.at(i)->getCible(), p, target.at(i)->getChoix());
    }
}
void Carte::jouerEffetsAllies(Partie* p,std::vector<ChoixEffet*> allies){
    if (allies.size() != effetsAllie.size()) {
        throw std::invalid_argument("Le nombre de cibles alliées ne correspond pas au nombre d'effets alliés.");
    }
    for (size_t i = 0; i < effetsAllie.size(); ++i) {
        effetsAllie.at(i).appliquer(this, allies.at(i)->getCible(), p, allies.at(i)->getChoix());
    }
}