#include "Joueur.h"
#include "../view/console/cardRenderer.h"


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
            while (defausse.getTaille() > 0) {
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
bool Joueur::jouerCarte(int idx,Partie &p,std::vector<ChoixEffet*> target,std::vector<ChoixEffet*> targetAllies){
    if (idx < 0 || idx >= main.getTaille()) {
        throw std::out_of_range("Index de carte invalide");
        return false;
    }
    Carte* c = main.getCarte(idx);
    if (c != nullptr) {
        if (c->getEstJouee()) {
            return false;
        }
        if (dynamic_cast<Champion*>(c)) {
            champions.push_back(dynamic_cast<Champion*>(c));
            main.defausser(idx); // Retirer de la main sans aller dans la défausse
        }
        c->lorsDeJeu(p,target,targetAllies,possedeAllie(*c));
        c->setEstJouee(true);
        
        return true;
    }
    return false;
}
bool Joueur::mobiliserChampion(int idx,Partie &p,std::vector<ChoixEffet*> target){
    if (idx < 0 || idx >= (int)champions.size()) {
        throw std::out_of_range("Index de champion invalide");
        return false;
    }
    Champion* c = champions.at(idx);
    if (c != nullptr) {
        if (c->estMobilise()) {
            return false;
        }
        c->mobiliser(p,target);
        return true;
    }
    return false;
}

bool Joueur::acheterDepuisMarche(int slot,Partie& p){
    return p.getMarche()->acquerirDepuisLigne(slot,*this,p);
}

void Joueur::finTour(){
    main.viderMain(this);
    piocher(5);
    for (auto c : champions){
        c->demobiliser();
    }
}
bool Joueur::aGarde()const {
    for (auto c : champions){
        if (c->estGarde()){
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


void Joueur::retirerCarteDefausseVersDessusDeck(int index) {
    // Retirer la carte de la défausse
    Carte* carte = defausse.retirerCarte(index);
    
    if (carte == nullptr) {
        throw std::runtime_error("Carte retirée est nullptr");
    }
    
    // Ajouter au-dessus du deck (en fait en dessous car on pioche par le dessus)
    deck.ajouterDessous(carte);
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
    if (aGarde()){
        throw std::runtime_error("Impossible d'attaquer le joueur tant qu'un garde est mobilisé");
        return false;
    }
    else{
        pv-=m;
        return true;
    }
    
}

bool Joueur::possedeAllie(Carte& carte) {
    for (auto c:main.getCartes()) {
        if (c->getFaction() == carte.getFaction() && c->getId() != carte.getId()) {
            return true;
        }
    }
    for (auto c:champions) {
        if (c->getFaction() == carte.getFaction() && c->getId() != carte.getId()) {
            return true;
        }
    }
    return false;
}

int Joueur::getNombreCartesJouables() const {
    int count = 0;
    for (int i = 0; i < main.getTaille(); ++i) {
        Carte* carte = main.getCarte(i);
        if (carte && !carte->getEstJouee()) {
            count++;
        }
    }
    return count;
}

std::vector<Champion*> Joueur::getDefausseChampions() const {
    std::vector<Champion*> champions;
    for (Carte* carte : defausse.getCartes()) {
        Champion* champ = dynamic_cast<Champion*>(carte);
        if (champ) {
            champions.push_back(champ);
        }
    }
    return champions;
}