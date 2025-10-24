#include "marche.h"

Marche::Marche(){
    deckMarche = BibliothequeCartes::creerDeckMarche();
    deckMarche.melanger();
}

void Marche::remplirLigne(){
    size_t currentSize = ligne.size();
    for (size_t i = 0; i < 5-currentSize; i++) {
        ligne.push_back(deckMarche.piocher());
    }
}
bool Marche::acquerirDepuisLigne(int slot, Joueur& j, Partie& partie){
    Carte* c = ligne.at(slot);
    if (c==nullptr){
        return false;
    }
    if (partie.getTourCourant()->getOr() < c->getCout()) {
        return false;
    }
    ligne.erase(ligne.begin() + slot);
    j.getDefausse().ajouter(c);
    partie.getTourCourant()->utiliserOr(c->getCout());
    remplirLigne();
    return true;
}


bool Marche::acquerirDepuisDeck(int idx, Joueur& j){
    Carte* c = deckMarche.getCartes().at(idx);
    if (c==nullptr){
        return false;
    }
    j.getMain().ajouter(c);
    deckMarche.retirerCarte(idx);
    return true;
}
