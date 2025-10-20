#pragma once
#include "Carte.h"
#include "effet.h"
#include "faction.h"
#include <vector>
#include <string>


class Joueur;
class Partie;

class Champion : public Carte {

    int vie;
    bool garde; // Si vrai, le champion doit être attaqué avant les autres champions ou le joueur
    bool mobilise;
    std::vector<Effet> effetsActiver;

public:

    // Constructeur
    Champion(const std::string& nom, int cout, Faction f, bool garde, int vie) : Carte(nom, cout, f), vie(vie), garde(garde), mobilise(false), effetsActiver() {}

    // Getters & setters
    std::vector<Effet> getEffetsActiver() const {return effetsActiver;}
    Champion* ajouterEffetActiver(const Effet& e) { effetsActiver.push_back(e); return this; }
    bool estGarde() const { return garde; }
    void setGarde(bool g) { garde = g; }
    bool estMobilise() const { return mobilise; }
    void setMobilise(bool m) { mobilise = m; }
    int getVie() const { return vie; }
    void setVie(int v) { vie = v; }

    // Méthodes
    void mobiliser(Partie& p,std::vector<void*> target);
    void demobiliser();
};
