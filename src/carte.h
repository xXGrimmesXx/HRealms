#ifndef CARTE_H
#define CARTE_H
#include <string>
#include <vector>
#include "Faction.h"
#include "Effet.h"

class Joueur;
class Partie;

class Carte {
protected:
    int id;
    std::string nom;
    int cout;
    Faction faction;
    std::vector<Effet> effetsPrimaire;
    std::vector<Effet> effetsAllie;
    std::vector<Effet> effetsActiver;
    std::vector<Effet> effetsSacrifice;

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
    std::vector<Effet> getEffetsActiver() const {return effetsActiver;}
    std::vector<Effet> getEffetsSacrifice() const {return effetsSacrifice;}

    // Setters
    Carte* ajouterEffetPrimaire(const Effet& e) { effetsPrimaire.push_back(e); return this; }
    Carte* ajouterEffetAllie(const Effet& e) { effetsAllie.push_back(e); return this; }
    Carte* ajouterEffetActiver(const Effet& e) { effetsActiver.push_back(e); return this; }
    Carte* ajouterEffetSacrifice(const Effet& e) { effetsSacrifice.push_back(e); return this; }

    // Méthodes
    virtual void lorsDeJeu(Joueur& j, Partie& p) = 0;
    bool peutDeclencherAllie(Joueur& j);
    virtual void activer(Joueur& j, Partie& p);
    virtual void sacrifier(Joueur& j, Partie& p);
};

#endif