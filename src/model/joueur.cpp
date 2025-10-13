#include "Joueur.h"

Joueur::Joueur(std::string nom) {
    this->nom = nom;
    pv = 50;
    deck = BibliothequeCartes::creerDeckDepart();
    deck.melanger();
    prochainAchatActionDessusDeck = false;
    prochainAchatDansMain = false;
    main = Main();
    defausse = Pile();
    champions = std::vector<Champion*>();
    piocher(5); // Piocher 5 cartes au début
}

void Joueur::piocher(int n){
    for (int i = 0; i < n; ++i) {
        if (deck.estVide()) {
            // Mélanger la défausse dans le deck
            while (defausse.taille() > 0) {
                Carte* c = defausse.prendreDessus();
                deck.ajouterDessous(c);
            }
            deck.melanger();
        }
        Carte* c = deck.piocher();
        if (c != nullptr) {
            main.ajouter(c);
        }
    }
}
bool Joueur::jouerCarte(int idx,Partie &p,std::vector<void*> target){
    if (idx < 0 || idx >= main.getTaille()) {
        throw std::out_of_range("Index de carte invalide");
        return false;
    }
    Carte* c = main.getCarte(idx);
    if (c != nullptr) {
        c->lorsDeJeu(p,target);
        main.defausser(idx);
        defausse.ajouter(c);
        std::cout << main<<std::endl;
        return true;
    }
    return false;
}
bool Joueur::acheterDepuisMarche(int slot,Partie& p){
    return p.getMarche()->acquerirDepuisLigne(slot,*this,p);
}

void Joueur::acheterGemmeFeu(Partie& p){
    p.getMarche()->acquerirGemmeFeu(*this,p);
}

void Joueur::finTour(){
    main.viderMain(this);
}
bool Joueur::aGardeMobilise()const {
    for (auto c : champions){
        if (c->estMobilise()){
            return true;
        }
    }
    return false;
 }
void Joueur::soigner(int montant){
    pv+=montant;
}

void Joueur::defausser(int index){
    if (index < 0 || index >= (int)main.getCartes().size()) {
        throw std::out_of_range("Index de carte invalide");
    }
    main.defausser(index);
}
void Joueur::assomerChampions(){
    for(auto c : champions){
        defausse.ajouter(c);
    }
    champions.clear();
}
void Joueur::assomerChampion(Champion* c){
    for (int i=0; i<(int)champions.size();i++){
        if (champions.at(i)==c){
            defausse.ajouter(c);
            champions.erase(std::next(champions.begin(),i));
        }
    }
}

void Joueur::choisirCarteDefausseAjouterDessusDeck(int montant){
    for(int i=0 ; i<montant; i++){

    }
}
void Joueur::choisirChampionDefausseAjouterDessusDeck(int montant){
    for (int i=0; i<montant; i++){

    }

}

void Joueur::sacrifier(int montant, Partie& p){
    for(int i=0; i<montant; i++){
        std::cout << "choisissez une carte à sacrifier"<<std::endl;
        for (int j =0; j<(int)main.getCartes().size();j++){
            std::cout<<i<<")   ";
        }
        std::cout <<std::endl;
        int choix;
        std::cin >>choix;
        while(choix<0 || choix >=(int)main.getCartes().size()){
            std::cout<<"choisissez un indice valide"<<std::endl;
            std::cin >> choix;
        }
        Carte* c = main.getCarte(choix);
        main.sacrifier(c);
        p.getCartesSacrifie().ajouter(c);
    }
}
std::vector<Champion*> Joueur::getGardes()const{
    std::vector<Champion*> res;
    for (auto c : champions){
        if (c->estGarde()){
            res.push_back(c);
        }
    }
    return res;
}

bool Joueur::prendreDegats(int m){
    if (aGardeMobilise()){
        throw std::runtime_error("Impossible d'attaquer le joueur tant qu'un garde est mobilisé");
        return false;
    }
    else{
        pv-=m;
        return true;
    }
    
}