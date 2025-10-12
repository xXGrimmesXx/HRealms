#include "deck.h"


Deck::Deck():cartes(std::deque<Carte*>()){}

Deck::~Deck(){}

void Deck::melanger(){
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(cartes.begin(), cartes.end(), g);
}

Carte* Deck::piocher(){
    Carte * c = cartes.back();
    cartes.pop_back();
    return c;
}
void Deck::ajouterDessous(Carte* c){
    cartes.push_front(c);
}
bool Deck::estVide() const{
    return cartes.empty();
}
void Deck::rechargerDepuis(Pile& defausse){
    for (auto c : defausse.getCartes()){
        cartes.push_front(c);
    }
}