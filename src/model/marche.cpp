#include "marche.h"

void Marche::remplirLigne(){}
bool Marche::acquerirDepuisLigne(int slot, Joueur& j, Partie& partie){
    std::cout <<slot<<j<<partie;
    return true;
}
bool Marche::acquerirGemmeFeu(Joueur& j, Partie& partie){
    std::cout <<j<<partie;
    return true;
}
bool Marche::acquerirDepuisDeck(int idx, Joueur& j, Partie& partie){
    std::cout<<idx<<j<<partie;
    return true;
}
void Marche::retournerGemmeFeuDansPile(Carte* g){
    std::cout<<g;
}