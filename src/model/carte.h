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
    bool effetPrimaireActivé;
    bool effetAllieActivé;

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
    bool getEffetPrimaireActive() const { return effetPrimaireActivé; }
    bool getEffetAllieActive() const { return effetAllieActivé; }

    // Setters
    Carte* ajouterEffetPrimaire(const Effet& e) { effetsPrimaire.push_back(e); return this; }
    Carte* ajouterEffetAllie(const Effet& e) { effetsAllie.push_back(e); return this; }
    Carte* ajouterEffetSacrifice(const Effet& e) { effetsSacrifice.push_back(e); return this; }
    bool setEffetPrimaireActive(bool b) { return effetPrimaireActivé = b; }
    bool setEffetAllieActive(bool b) { return effetAllieActivé = b; }
    void setId(int i) { id = i; }

    // Méthodes
    void jouerEffetPrimaires(Partie* p, std::vector<ChoixEffet*> target);
    void jouerEffetsAllies(Partie* p,std::vector<ChoixEffet*> allies);
    void sacrifier(std::vector<ChoixEffet*> cibles, Partie* p);

    // Helpers pour l'affichage
    std::string to_string() const {
        return nom + " (Coût: " + std::to_string(cout) + ", Faction: " + std::to_string(static_cast<int>(faction)) + ")";
    }
};

#endif