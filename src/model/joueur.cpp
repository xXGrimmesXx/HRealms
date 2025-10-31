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
bool Joueur::jouerCarte(Carte* c,Partie *p,std::vector<ChoixEffet*> target,std::vector<ChoixEffet*> targetAllies){
    if (c == nullptr) {
        throw std::invalid_argument("Carte invalide");
        return false;
    }
    if (c->getEstJouee()) {
    return false;
    }
    if (dynamic_cast<Champion*>(c)) {
        champions.push_back(dynamic_cast<Champion*>(c));
        main.defausser(c); // Retirer de la main sans aller dans la défausse
    }
    c->jouerEffetPrimaires(p,target);
    if (!targetAllies.empty()) {
        c->jouerEffetsAllies(p,targetAllies);
    }
    c->setEstJouee(true);
    
    return true;
}
bool Joueur::mobiliserChampion(int idx,Partie *p,std::vector<ChoixEffet*> target){
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

bool Joueur::aChampionMobilisable(){
    for (auto c : champions){
        if (!c->estMobilise()){
            return true;
        }
    }
    return false;
}

bool Joueur::acheterDepuisMarche(int slot,Partie* p){
    return p->getMarche()->acquerirDepuisLigne(slot,this,p);
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

void Joueur::defausser(Carte * c){
    if (c == nullptr) {
        throw std::invalid_argument("Carte invalide");
    }
    main.defausser(c);
}
void Joueur::sacrifier(Carte* c,Partie* p){
    if (c == nullptr) {
        throw std::invalid_argument("Carte invalide");
    }
    main.sacrifier(c);

    //si la carte est une gemme de feu, la remettre dans la pile gemme de feu
    //sinon on la met dans la pile de cartes sacrifiées
    std::cout << "Sacrifier la carte: " << c->to_string() <<std::endl<< "est une gemme de feu " << (c->getNom() == "Gemme de feu") <<std::endl;
    if (c->getNom() == "Gemme de feu") {
        p->getGemmeDeFeu()->getPile()->ajouter(c);
    }
    else{
        p->getCartesSacrifie()->ajouter(c);
    }
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
            std::cout << champions.size() << " champions restants pour le joueur joueur.cpp " << getNom() << std::endl;
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

bool Joueur::possedeAllie(Carte* carte) {
    for (auto c:main.getCartes()) {
        if (c->getFaction() == carte->getFaction() && c->getId() != carte->getId()) {
            return true;
        }
    }
    for (auto c:champions) {
        if (c->getFaction() == carte->getFaction() && c->getId() != carte->getId()) {
            return true;
        }
    }
    return false;
}

int Joueur::getNombreCartesJouables() const {
    int count = 0;
    for (int i = 0; i < main.getTaille(); ++i) {
        Carte* carte = main.getCarte(i);
        if (carte !=nullptr && !carte->getEstJouee()) {
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

std::vector<Champion*> Joueur::getChampionsMain() const {
    std::vector<Champion*> championsMain;
    for (Carte* carte : main.getCartes()) {
        Champion* champ = dynamic_cast<Champion*>(carte);
        if (champ != nullptr) {
            championsMain.push_back(champ);
        }
    }
    return championsMain;
}

std::vector<Champion*> Joueur::getGardesMain() const {
    std::vector<Champion*> gardesMain;
    for (Carte* carte : main.getCartes()) {
        Champion* champ = dynamic_cast<Champion*>(carte);
        if (champ != nullptr && champ->estGarde()) {
            gardesMain.push_back(champ);
        }
    }
    return gardesMain;
}

Carte* Joueur::retirerCarteAvecEffetSacrifice(int globalIdx, bool& fromChampions) {
    // Récupère toutes les cartes avec effetSacrifice
    std::vector<Carte*> candidates;
    for (int i = 0; i < main.getTaille(); ++i) {
        if (main.getCarte(i)->getEffetsSacrifice().size() > 0) {
            candidates.push_back(main.getCarte(i));
        }
    }
    for (auto champ : champions) {
        if (champ->getEffetsSacrifice().size() > 0) {
            candidates.push_back(champ);
        }
    }

    if (globalIdx < 0 || globalIdx >= (int)candidates.size()) return nullptr;
    Carte* c = candidates.at(globalIdx);

    // Retire la carte de la bonne zone
    if (std::find(main.getCartes().begin(), main.getCartes().end(), c) != main.getCartes().end()) {
        main.sacrifier(c);
        fromChampions = false;
    } else {
        auto it = std::find(champions.begin(), champions.end(), c);
        if (it != champions.end()) {
            champions.erase(it);
            fromChampions = true;
        }
    }
    return c;
}

// Setters pour la restauration depuis JSON
void Joueur::setMain(Main* m) {
    main = *m;
}

void Joueur::setDeck(Deck* d) {
    deck = *d;
}

void Joueur::setDefausse(Pile* p) {
    defausse = *p;
}