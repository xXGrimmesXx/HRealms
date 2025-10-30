#include "deck.h"


Deck::Deck():cartes(std::deque<Carte*>()){
    auto rd = std::random_device {};
    rng = std::default_random_engine {rd()};
}

Deck::~Deck(){}

void Deck::melanger(){
    if (cartes.empty()) {
        return;
    }
    for (size_t i = cartes.size() - 1; i > 0; --i) {
        std::uniform_int_distribution<size_t> dist(0, i);
        size_t j = rng() % (i + 1); 
        //size_t j = dist(rng);  // ← Cette ligne bloque/plante
        std::swap(cartes[i], cartes[j]);
    }
}

Carte* Deck::piocher(){
    if (cartes.empty()) {
        return nullptr; // ou gérer l'erreur selon vos besoins
    }
    else{
    Carte * c = cartes.back();
    cartes.pop_back();
    return c;
    }
}
void Deck::ajouterDessous(Carte* c){
    c->setEstJouee(false);
    cartes.push_front(c);
}
void Deck::ajouterDessus(Carte* c){
    c->setEstJouee(false);
    cartes.push_back(c);
}
bool Deck::estVide() const{
    return cartes.empty();
}
void Deck::rechargerDepuis(Pile* defausse){
    for (auto c : defausse->getCartes()){
        c->setEstJouee(false);
        cartes.push_front(c);
    }
}

void Deck::retirerCarte(int idx){
    if (idx >= 0 && idx < static_cast<int>(cartes.size())) {
        cartes.erase(cartes.begin() + idx);
    }
}