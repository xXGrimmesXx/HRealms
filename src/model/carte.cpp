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
bool Carte::peutDeclencherAllie(Joueur& j) {
    // Joueur et Main restent à définir
    return j.getMain().contientCarteDeFaction(faction);
}

void Carte::sacrifier(Joueur& j, Partie& p) {
    // Effet reste à définir
    for (const auto& effet : effetsSacrifice) {
        effet.appliquer(this, {}, p);
    }
    j.getMain().sacrifier(this);
    p.getCartesSacrifie().ajouter(this);
}

void Carte::activer(Partie& p,Joueur& joueurSource,void* target){
    for (const Effet& effet : effetsAllie) {
        int nbAllies=0;
        for(Champion* c : joueurSource.getChampions()){
            if(c->getFaction() == this->getFaction()){
                nbAllies++;
            }
        }
        for (Carte* c : joueurSource.getMain().getCartes()) {
            if (c->getFaction() == this->getFaction() && c != this) {
                nbAllies++;
            }
        }
        effet.appliquer(this,target, p);
    }
    effetAllieDeclenche=true;
}

void Carte::lorsDeJeu(Partie& p, void* target) {
    for (const Effet& effet : effetsPrimaire) {
        effet.appliquer(this, target, p);
    }
}
