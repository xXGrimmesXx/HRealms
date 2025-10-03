#pragma once

class Joueur;

class Attaquable {
public:
    virtual ~Attaquable() = default;
    virtual void prendreDegats(int m) = 0;
    virtual Joueur* proprietaire() = 0;
};
