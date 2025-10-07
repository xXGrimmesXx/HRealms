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
    int Carte::getId() const { return id; }
    std::string Carte::getNom() const { return nom; }
    int Carte::getCout() const { return cout; }
    Faction Carte::getFaction() const { return faction; }
    std::vector<Effet> Carte::getEffetsPrimaire() const {return effetsPrimaire};
    std::vector<Effet> Carte::getEffetsAllie() const {return effetsAllie};
    std::vector<Effet> Carte::getEffetsActiver() const {return effetsActiver};
    std::vector<Effet> Carte::getEffetsSacrifice() const {return effetsSacrifice};
    
    // Setters
    Carte* ajouterEffetPrimaire(const Effet& e) { effetsPrimaire.push_back(e); }
    Carte* ajouterEffetAllie(const Effet& e) { effetsAllie.push_back(e); }
    Carte* ajouterEffetActiver(const Effet& e) { effetsActiver.push_back(e); }
    Carte* ajouterEffetSacrifice(const Effet& e) { effetsSacrifice.push_back(e); }

    // Méthodes
    virtual void lorsDeJeu(Joueur& j, Partie& p) = 0;
    bool peutDeclencherAllie(Joueur& j);
    virtual void activer(Joueur& j, Partie& p);
    virtual void sacrifier(Joueur& j, Partie& p);
};

#endif