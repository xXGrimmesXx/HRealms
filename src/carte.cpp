#include "Carte.h"
#include "Joueur.h"
#include "Partie.h"

Carte::Carte(int id, const std::string& nom, int cout, Faction f)
    : id(id), nom(nom), cout(cout), faction(f) {}

bool Carte::peutDeclencherAllie(Joueur& j) {
    // Joueur et Main restent à définir
    return j.getMain().contientCarteDeFaction(faction);
}

void Carte::activer(Joueur& j, Partie& p) {
    // Effet reste à définir
    for (const auto& effet : effetsActiver) {
        // {} = pas de cibles, mais du coup on doit adapter en fonction des effets
        effet.appliquer(&j, {}, p);
    }
}

void Carte::sacrifier(Joueur& j, Partie& p) {
    // Effet reste à définir
    for (const auto& effet : effetsSacrifice) {
        effet.appliquer(&j, {}, p);
    }
}
