#include "marche.h"

void Marche::remplirLigne(){}
void Marche::acquerirDepuisLigne(int slot, Joueur& j, Partie& partie){
    std::cout <<slot<<j<<partie;
}
void Marche::acquerirGemmeFeu(Joueur& j, Partie& partie){
    std::cout <<j<<partie;
}
void Marche::acquerirDepuisDeck(int idx, Joueur& j, Partie& partie){
    std::cout<<idx<<j<<partie;
}
void Marche::retournerGemmeFeuDansPile(Carte* g){
    std::cout<<g;
}