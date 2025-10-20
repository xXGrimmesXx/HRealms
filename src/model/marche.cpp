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
    ligne.erase(ligne.begin() + slot);
    j.getDefausse().ajouter(c);
    partie.getTourCourant()->utiliserOr(c->getCout());
    remplirLigne();
    return true;
}
bool Marche::acquerirGemmeFeu(Joueur& j, Partie& partie){
    std::cout << "Acquisition de gemme de feu non implémentée." << std::endl;
    std::cout << j << " " << partie << std::endl;
    return false;
}
bool Marche::acquerirDepuisDeck(int idx, Joueur& j, Partie& partie){
    std::cout << "Acquisition depuis le deck du marché non implémentée." << std::endl;
    std::cout << idx << " " << j << " " << partie << std::endl;
    return true;
}
void Marche::retournerGemmeFeuDansPile(Carte* g){
    pileGemmesFeu.ajouter(g);
}