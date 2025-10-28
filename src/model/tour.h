#pragma once


class Joueur;

class Tour {
private:
    Joueur* joueur;
    int reserveOr;
    int reserveDegats;

public:
    Tour(Joueur* j);
    void ajouterOr(int m);
    void ajouterDegats(int m);
    void reinitialiser();

    // Getters et setters
    int getOr() const;
    int getDegats() const;
    Joueur* getJoueur() const;
    void setJoueur(Joueur* j);
    
    void utiliserDegats(int m);
    void utiliserOr(int m);
};
