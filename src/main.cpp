#include "main.h"

void Main::ajouter(Carte* c){
    cartes.push_back(c);
}

void Main::defausser(int idx){
    if(idx < 0 || idx >= cartes.size()) {
        throw std::out_of_range("Index hors limites");
    }
    delete cartes[idx];
    cartes.erase(cartes.begin() + idx);
}

void Main::viderMain(){
    for(Carte* c : cartes) {
        delete c;
    }
    cartes.clear();
}

void Main::jouerCarte(int idx){
    if(idx < 0 || idx >= cartes.size()) {
        throw std::out_of_range("Index hors limites");
    }
    Carte* c = cartes[idx];
    cartes.erase(cartes.begin() + idx);
}

void Main::toString() const{
    std::cout << "Main: \n";
    int i=1;
    for(const Carte* c : cartes) {
        std::cout <<  i << ") " << c->getNom() << " ";
        ++i;
    }
    std::cout << std::endl;
}

