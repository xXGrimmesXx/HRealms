#pragma once
#include <string>
#include <vector>
#include "Deck.h"
#include "Pile.h"
#include "Main.h"
#include "Champion.h"
#include "Attaquable.h"
#include "partie.h"
#include "bibliothequeCartes.h"

class Deck;

class Joueur : public Attaquable {
private:
    std::string nom;
    int pv;
    Deck deck;
    Pile defausse;
    Main main;
    std::vector<Champion*> champions;
    bool prochainAchatActionDessusDeck;
    bool prochainAchatDansMain;

public:
    Joueur(std::string nom="");
    ~Joueur() = default;
    void piocher(int n);
    bool jouerCarte(int idx,Partie& p,std::vector<void*> target,std::vector<void*> targetAllies);
    bool acheterDepuisMarche(int slot, Partie& p);
    void finTour();
    bool aGardeMobilise() const;
    void soigner(int montant);
    void defausser(int n);
    void assomerChampions();
    void assomerChampion(Champion* c);
    void activerProchainAchatActionDessusDeck(){prochainAchatActionDessusDeck=true;}
    void activerProchainAchatDansMain(){prochainAchatDansMain=true;}
    void sacrifier(int montant, Partie& p);
    std::vector<Champion*> getGardes()const;
    // TODO
    
    
    void choisirCarteDefausseAjouterDessusDeck(int montant);
    void choisirChampionDefausseAjouterDessusDeck(int montant);


    // interface Attaquable
    bool prendreDegats(int m) override;
    Joueur* proprietaire() override { return this; }

    // getters et setters
    std::string getNom() const { return nom; }
    Main& getMain() { return main; }
    Deck& getDeck() { return deck; }
    Pile& getDefausse() { return defausse; }
    std::vector<Champion*>& getChampions() { return champions; }
    const std::vector<Champion*>& getChampions() const { return champions; }
    void setPv(int v) { pv = v; }
    int getPv() const { return pv; }

    std::string toString() const {
        return nom + " (PV: " + std::to_string(pv) + ", Defausse: " + std::to_string(defausse.getTaille()) + " cartes, Champions: " + std::to_string(champions.size()) + ")";
    }
    friend std::ostream& operator<<(std::ostream& os, const Joueur& j) {
        os << j.toString();
        return os;
    }
};
