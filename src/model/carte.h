#ifndef CARTE_H
#define CARTE_H
#include <string>
#include <vector>
#include "effet.h"
#include "Faction.h"
#include "choixEffet.h"

class Joueur;
class Partie;

class Carte {
protected:
    int id;
    std::string nom;
    int cout;
    Faction faction;
    int bonusParAllie;
    std::vector<Effet> effetsPrimaire;
    std::vector<Effet> effetsAllie;
    std::vector<Effet> effetsSacrifice;
    bool estJouee;

public:
    // Constructeur & destructeur
    // On a préféré rajouter les effets avec les setters, pour une meilleure lisibilté lors de la construction de BibliothequeCartes
    Carte(std::string nom, int cout, Faction f);
    virtual ~Carte() = default;

    // Getters
    int getId() const { return id; }
    std::string getNom() const { return nom; }
    int getCout() const { return cout; }
    Faction getFaction() const { return faction; }
    std::vector<Effet> getEffetsPrimaire() const {return effetsPrimaire;}
    std::vector<Effet> getEffetsAllie() const {return effetsAllie;}
    std::vector<Effet> getEffetsSacrifice() const {return effetsSacrifice;}
    bool getEstJouee() const { return estJouee; }

    // Setters
    Carte* ajouterEffetPrimaire(const Effet& e) { effetsPrimaire.push_back(e); return this; }
    Carte* ajouterEffetAllie(const Effet& e) { effetsAllie.push_back(e); return this; }
    Carte* ajouterEffetSacrifice(const Effet& e) { effetsSacrifice.push_back(e); return this; }
    bool setEstJouee(bool b) { return estJouee = b; }

    // Méthodes
    void lorsDeJeu(Partie& p,std::vector<ChoixEffet*> target, std::vector<ChoixEffet*> allies,bool declencherAllie);
    bool peutDeclencherAllie(Joueur& j);
    void activer(Partie& p,Joueur& joueurSource,void* target);
    void sacrifier(Joueur& j, Partie& p);
};

#endif