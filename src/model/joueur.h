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
#include "choixEffet.h"


class Deck;
class CardRenderer;
class ChoixEffet;

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
    // Constructeur
    Joueur(std::string nom="");
    ~Joueur() = default;

    // Méthodes
    void piocher(int n);
    bool jouerCarte(Carte* c,Partie* p,std::vector<ChoixEffet*> target,std::vector<ChoixEffet*> targetAllies);
    bool mobiliserChampion(int idx, Partie* p,std::vector<ChoixEffet*> target);
    bool acheterDepuisMarche(int slot, Partie* p);
    void finTour();
    bool aChampionMobilisable();
    bool aGarde() const;
    void soigner(int montant);
    void defausser(Carte * c);
    void sacrifier(Carte*c,Partie* p);
    void assomerChampions();
    void assomerChampion(Champion* c);
    void activerProchainAchatActionDessusDeck(){prochainAchatActionDessusDeck=true;}
    void desactiverProchainAchatActionDessusDeck(){prochainAchatActionDessusDeck=false;}
    void activerProchainAchatDansMain(){prochainAchatDansMain=true;}
    void desactiverProchainAchatDansMain(){prochainAchatDansMain=false;}
    bool possedeAllie(Carte* carte);
    void retirerCarteDefausseVersDessusDeck(int index);
    Carte* retirerCarteAvecEffetSacrifice(int globalIdx, bool& fromChampions);

    // interface Attaquable
    bool prendreDegats(int m) override;
    Joueur* proprietaire() override { return this; }

    // getters et setters
    std::string getNom() const { return nom; }
    Main* getMain() { return &main; }
    Deck* getDeck() { return &deck; }
    Pile* getDefausse() { return &defausse; }
    std::vector<Champion*> getChampions() { return champions; }
    void ajouterChampion(Champion* c){champions.push_back(c);}
    void setChampions(const std::vector<Champion*>& c) { champions = c; }
    void setMain(Main* m);
    void setDeck(Deck* d);
    void setDefausse(Pile* p);
    std::vector<Champion*> getGardes()const;
    void setPv(int v) { pv = v; }
    int getPv() const { return pv; }
    bool getProchainAchatActionDessusDeck() const { return prochainAchatActionDessusDeck; }
    bool getProchainAchatDansMain() const { return prochainAchatDansMain;}
    void setProchainAchatActionDessusDeck(bool b) { prochainAchatActionDessusDeck = b; }
    void setProchainAchatDansMain(bool b) { prochainAchatDansMain = b; }
    std::vector<Champion*> getDefausseChampions() const;
    std::vector<Champion*> getChampionsMain() const;
    std::vector<Champion*> getGardesMain() const;
    int getNombreCartesJouables() const;


    // Helpers pour l'affichage
    std::string toString() const {
        return nom + " (PV: " + std::to_string(pv) + ", Defausse: " + std::to_string(defausse.getTaille()) + " cartes, Champions: " + std::to_string(champions.size()) + ")";
    }
    friend std::ostream& operator<<(std::ostream& os, const Joueur& j) {
        os << j.toString();
        return os;
    }
};
